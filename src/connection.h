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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <pthread.h>

#if defined(__GNUC__)
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#include <tr1/unordered_map>
#define hash_map_t std::tr1::unordered_map
#else
#include <ext/hash_map>
#define hash_map_t __gnu_cxx::hash_map
#endif
#elif defined(_MSC_VER)
#include <hash_map>
#define hash_map_t stdext::hash_map
#endif

#include <google/protobuf/service.h>

#include "proto/comm.pb.h"

namespace trpc {

/*
 * Implements the Trivial RPC protocol over a raw socket
 *
 * Trivial RPC communication is implemented using Google Protocol
 * Buffers. See the protocol specification in proto/comm.proto and
 * the RPC service specification in proto/rpc.proto and the PB website
 * http://code.google.com/apis/protocolbuffers/ for more details.
 *
 * TODO: 
 *   - Protocol support for multiple services
 *   - Asynchronous RPC?
 */

class Connection {
 public:
    Connection(int socket);
    ~Connection();

    // Set the service handled by this connection
    void setService(google::protobuf::Service *);
    // Get the service handled by this connection
    google::protobuf::Service * getService() const;
  
    // Reads and handles messages from the connection
    // 
    // Will only return at the end of the Stream, which
    // means socket shutdown, or in error (returns false)
    bool runEventLoop(); 

    // Callback used by RPC channel for notification of
    // call response
    class DoneCallback {
     public:
        DoneCallback(proto::CallResponse * r);
        ~DoneCallback();

        void waitUntilDone();
        void run();

        proto::CallResponse * response;
        bool done;
        pthread_mutex_t done_lock;
        pthread_cond_t done_cv;
    };

    bool registerCall(int call_id, DoneCallback * cb);

    // Send a message over the connection
    bool writeMessage(int fieldNum, google::protobuf::Message *); 

 private:
    bool readMessage();

    void sendError(proto::Error::Type, std::string);

    bool handleMessage(proto::SessionHeader &);
    bool handleMessage(proto::Error &);
    bool handleMessage(proto::CallRequest &);
    bool handleMessage(proto::CallResponse &);

    int m_socket;
    google::protobuf::Service * m_service;

    hash_map_t<int, DoneCallback *> m_calls;
};

}


#endif
