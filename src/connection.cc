/* 
 * Copyright (c) 1996-2010 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>

#include <string>

#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/descriptor.h>

#include "connection.h"
#include "rpc_channel.h"
#include "rpc_controller.h"
#include "debug.h"

using namespace trpc;

Connection::Connection(int socket)
  : m_socket(socket)
{ }
Connection::~Connection()
{ }

void
Connection::setService(google::protobuf::Service * s)
{
    m_service = s;
}

google::protobuf::Service *
Connection::getService() const {
    return m_service;
}

bool
Connection::runEventLoop()
{
    while(readMessage()) { } 
    return true;
}

namespace {
uint32_t mktag(int fn, int wiretype) {
    return fn << 3 | wiretype;
}
bool skipField(google::protobuf::io::CodedInputStream * cs, uint32_t tag)
{
    using namespace google::protobuf::internal;

    uint32_t unused_32;
    uint64_t unused_64;

    switch(tag & 0x7) {
        case WireFormatLite::WIRETYPE_VARINT:
            (void)cs->ReadVarint32(&unused_32);
            return true;
        case WireFormatLite::WIRETYPE_FIXED64:
            (void)cs->ReadLittleEndian64(&unused_64);
            return true;
        case WireFormatLite::WIRETYPE_LENGTH_DELIMITED:
            (void)cs->ReadVarint32(&unused_32);
            (void)cs->Skip(unused_32);
            return true;
        case WireFormatLite::WIRETYPE_START_GROUP:
            assert(0 && "lazily not handled");
            return true;
        case WireFormatLite::WIRETYPE_END_GROUP:
            return false;
        case WireFormatLite::WIRETYPE_FIXED32:
            (void)cs->ReadLittleEndian32(&unused_32);
            return true;
        default:
            assert(0 && "bad wire type"); 
    }
    return false;
}
}

bool
Connection::writeMessage(int fieldNum, google::protobuf::Message * m)
{
    using namespace google::protobuf::io;
    using namespace google::protobuf::internal;

    FileOutputStream fout(m_socket);
    CodedOutputStream output(&fout);

    output.WriteTag(mktag(fieldNum,WireFormatLite::WIRETYPE_LENGTH_DELIMITED));
    output.WriteVarint32(m->ByteSize());
    m->SerializeToCodedStream(&output);

    return true; // FIXME really?
}

void
Connection::sendError(proto::Error::Type type,std::string errstr)
{
    proto::Error err;

    err.set_type(type);
    err.set_description(errstr);

    writeMessage(proto::Stream::kErrorFieldNumber,&err);
}

bool
Connection::readMessage()
{
    using namespace google::protobuf::io;
    
    uint32_t size, tag, tagnum;
    CodedInputStream::Limit limit;
    char buf[64];
    bool ret = true;

    // only one will be filled in
    proto::SessionHeader sh;
    proto::CallRequest creq;
    proto::CallResponse cresp;
    proto::Error err;

    FileInputStream fin(m_socket);
    CodedInputStream input(&fin);

    tag = input.ReadTag();

    if(tag == 0)
        return false;

    tagnum = tag >> 3;

    trpc_printf("Read tag %d, tagnum %d, low bits %d\n",tag,tagnum,tag&0x7);

    // check wire type -- must be 2 for encapsulated messages
    if((tag & 0x7) != 2) {
        if(!skipField(&input,tag)) {
            fprintf(stderr,"Unexpected end-group tag\n");
            return false;
        }
   
        snprintf(buf,64,"Received non message tag %d",tag); 
        sendError(proto::Error::UNKNOWN_MESSAGE,std::string(buf));
    
        return true;
    }

    if(!input.ReadVarint32(&size)) {
        sendError(proto::Error::PROTOCOL_ERROR,"Failed to read message size");
        return false;
    }
    limit = input.PushLimit(size);

    switch(tagnum) {
        case proto::Stream::kHeaderFieldNumber:
            if(!sh.ParseFromCodedStream(&input)) {
                sendError(proto::Error::PROTOCOL_ERROR,"Bad session header");
                return false;
            }
            ret = handleMessage(sh);
            break;
        case proto::Stream::kCallRequestFieldNumber:
            if(!creq.ParseFromCodedStream(&input)) {
                sendError(proto::Error::PROTOCOL_ERROR,"Bad call request");
                return false;
            }
            ret = handleMessage(creq);
            break;
        case proto::Stream::kCallResponseFieldNumber:
            if(!cresp.ParseFromCodedStream(&input)) {
                sendError(proto::Error::PROTOCOL_ERROR,"Bad call response");
                return false;
            }
            ret = handleMessage(cresp);
            break;
        case proto::Stream::kErrorFieldNumber:
            if(!err.ParseFromCodedStream(&input)) {
                sendError(proto::Error::PROTOCOL_ERROR,"Bad error message");
                return false;
            }
            ret = handleMessage(err);
            break;
        default:
            snprintf(buf,64,"Received unknown field number %d",tagnum);
            sendError(proto::Error::UNKNOWN_MESSAGE,std::string(buf));
            input.Skip(size);
            break;
    }
    
    input.PopLimit(limit);

    return ret;
}

bool
Connection::handleMessage(proto::SessionHeader &)
{
    // not currently implemented in the protocol
    return true;
}

bool
Connection::handleMessage(proto::Error & err)
{
    fprintf(stderr,"RPC error: %s\n",err.description().c_str());
    return true;
}

bool
Connection::handleMessage(proto::CallRequest & creq)
{
    using namespace google::protobuf;

    SocketRpcController controller;
    proto::CallResponse rpc_response;
    char buf[64];

    const MethodDescriptor * md = m_service->GetDescriptor()->method(
        creq.method_id());

    if(!md) {
        snprintf(buf,64,"Bad method id %d",creq.method_id());
        sendError(proto::Error::PROTOCOL_ERROR,std::string(buf));
        return false;
    }

    // unpack
    Message * request = m_service->GetRequestPrototype(md).New();
    Message * response = m_service->GetResponsePrototype(md).New();

    rpc_response.set_call_id(creq.call_id());

    if(request->ParseFromString(creq.payload().data())) {
        // _synchronously_ run the RPC
        m_service->CallMethod(md,&controller,request,response,NULL);

        if(!controller.Failed()) {
            rpc_response.mutable_payload()->set_data(
                response->SerializeAsString());
        } else {
            rpc_response.mutable_error()->set_type(
                proto::CallError::APPLICATION_ERROR);
            rpc_response.mutable_error()->set_description(
                controller.ErrorText());
        }
    } else {
        rpc_response.mutable_error()->set_type(proto::CallError::BAD_REQUEST);
        rpc_response.mutable_error()->set_description("Request parse failed");
    }

    writeMessage(proto::Stream::kCallResponseFieldNumber,&rpc_response);

    delete request;
    delete response;

    return true;
}

bool
Connection::handleMessage(proto::CallResponse & cresp)
{
    // hmmm... how do we know what to run? --- the context knows.
    // the context is going to wait for its callback to be run,
    // which callback it presumably has placed on the connection's
    // list of callbacks.
    //
    // how can it wait on that while simultaneously processing connections?
    //
    // one way is to block on io, waiting for a message. so, client has
    // a connection that is in runLoop---that has to be a thread, otherwise
    // the client can't do any useful work! so:
    //
    // channel sets up condition variable
    // cannel passes a callback to connection that will kick the condition
    // variable
    // channel then blocks on that condition variable
    // when the connection receives the response, kick the client

    DoneCallback * done;
    char buf[64];

    if(m_calls.find(cresp.call_id()) == m_calls.end()) {
        // tell the server it's gone bonkers
        snprintf(buf,64,"Response received with bad call id %d",
            cresp.call_id());
        sendError(proto::Error::PROTOCOL_ERROR,std::string(buf));
        return false;
    }
    done = m_calls[cresp.call_id()];
    m_calls.erase(cresp.call_id());
    *(done->response) = cresp;
    done->run();
    return true;
}

bool
Connection::registerCall(int call_id, DoneCallback * cb)
{
    if(m_calls.find(call_id) != m_calls.end()) {
        fprintf(stderr,"Attempt to register in-use call_id %d\n",call_id);
        return false;
    }
    m_calls[call_id] = cb;
    return true;
}

Connection::DoneCallback::DoneCallback(proto::CallResponse * r)
  : response(r),
    done(false)
{
    pthread_mutex_init(&done_lock,NULL);
    pthread_cond_init(&done_cv,NULL);
}
Connection::DoneCallback::~DoneCallback()
{
    pthread_mutex_destroy(&done_lock);
    pthread_cond_destroy(&done_cv);
}

void
Connection::DoneCallback::waitUntilDone()
{
    pthread_mutex_lock(&done_lock);
    while(!done) {
        pthread_cond_wait(&done_cv,&done_lock);
    }
    pthread_mutex_unlock(&done_lock);
}

void
Connection::DoneCallback::run()
{
    pthread_mutex_lock(&done_lock);
    done = true;
    pthread_cond_signal(&done_cv);
    pthread_mutex_unlock(&done_lock);
}

