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
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "connection.h"
#include "hello_service.h"

/*
 * This trivial server example will listen for a client connection on a random
 * socket and invoke a HelloTest server using TRPC. When the client connetion
 * goes away, the server terminates.
 */

int s_listen = -1;     // listening socket
int s_client = -1;     // client socket

bool setup_listen()
{
    socklen_t sa_size;
    struct sockaddr_in addr;

    s_listen = socket(PF_INET, SOCK_STREAM, 0);

    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(0 != bind(s_listen,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)))
    {
        perror("bind");
        return false;
    }   

    if(0 != listen(s_listen, 10)) {
        perror("listen");
        return false;
    }   

    // see what listen() assigned for a port
    sa_size = sizeof(struct sockaddr_in);
    if(0 != getsockname(s_listen,(struct sockaddr*)&addr,&sa_size))
    {
        perror("getsockname");
        return false;
    }

    printf("test_server listening on port %d\n",ntohs(addr.sin_port));

    return true;
}

void serve()
{
    struct sockaddr_in ca;
    trpc::Connection * conn;
    trpc::hellotest::HelloServiceImpl server;

    socklen_t ca_sz = sizeof(struct sockaddr_in);
    s_client = accept(s_listen,(struct sockaddr*)&ca,&ca_sz);
    if(0 > s_client) {
        perror("accept");
        return;
    }

    shutdown(s_listen,SHUT_RDWR);
    close(s_listen);
    s_listen = -1;

    conn = new trpc::Connection(s_client);
    conn->setService(&server);
    conn->runEventLoop();

    delete conn;
}

int main(int argc, char**argv)
{
    // establish listening socket & listen
    if(setup_listen()) {
        serve();
    }

    if(s_listen > 0)
        close(s_listen);
    if(s_client > 0)
        close(s_client);

    return 0;
}

