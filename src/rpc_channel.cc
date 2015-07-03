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

#include <google/protobuf/descriptor.h>

#include "connection.h"
#include "rpc_channel.h"

using namespace trpc;

SocketRpcChannel::SocketRpcChannel(Connection * c) 
  : m_conn(c),
    m_next_call(0)
{
    
}

SocketRpcChannel::~SocketRpcChannel()
{ }

void 
SocketRpcChannel::CallMethod(
        const google::protobuf::MethodDescriptor * method,
        google::protobuf::RpcController * controller,
        const google::protobuf::Message * request,
        google::protobuf::Message * response,
        google::protobuf::Closure * done)
{
    proto::CallRequest creq;
    proto::CallResponse cresp;    
    proto::CallPayload * out_payload;

    Connection::DoneCallback rpc_done(&cresp);

    // build RPC request
    creq.set_call_id(m_next_call++);
    creq.set_method_id(method->index());
    out_payload = creq.mutable_payload();
    out_payload->set_data(request->SerializeAsString());

    // register call with Connection
    m_conn->registerCall(creq.call_id(),&rpc_done);

    if(!m_conn->writeMessage(proto::Stream::kCallRequestFieldNumber,&creq)) {
        // ack error! do something!
        fprintf(stderr,"UNHANDLED CALLMETHOD ERROR\n");
        assert(0);
    }

    // wait for response
    rpc_done.waitUntilDone();

    // parse the response
    if(cresp.has_error()) {
        controller->SetFailed(cresp.error().description());
    } else {
        response->ParseFromString(cresp.payload().data());
    }

    // kick done
    if(done)
        done->Run();
}

