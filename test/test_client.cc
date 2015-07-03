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
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

#include "connection.h"
#include "rpc_channel.h"
#include "rpc_controller.h"

#include "proto/hello.pb.h"

void usage(char *s)
{
    printf("Usage: %s [options] <port>\n",s);
}

/* getopt decl */
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

int parse_options(int argc, char**argv)
{
    int ch;
    
    static struct option long_options[] = {
        {"help",no_argument,0,'h'}
    };

    int option_index = 0;

    while((ch=
        getopt_long(argc,argv,"h",long_options,&option_index)) != -1)
    {
        switch(ch) {
            default:
                printf("Illegal option %c\n",ch);
            case 'h':
                usage(argv[0]);
                exit(1);
        }
    }

    return optind;
}

class ClientThread {
 public:
    ClientThread(trpc::Connection * c) : m_conn(c)
    { }

    void start() {
        fprintf(stderr,"ClientThread start() invoked\n");
        m_conn->runEventLoop();
        fprintf(stderr,"ClientThread RunEventLoop finished\n");
    }

 private:
    trpc::Connection * m_conn;
};

void * start_thread(void * arg) {
    ClientThread * ct = (ClientThread*)arg;
    ct->start(); 
    return NULL;
}

void run_tests(trpc::hellotest::HelloService_Stub & hs)
{
    trpc::SocketRpcController controller;
    trpc::hellotest::HelloRequest req;
    trpc::hellotest::HelloResponse resp;

    hs.Hello(&controller,&req,&resp,NULL);
    if(controller.Failed()) {
        printf("Hello failed: %s\n",controller.ErrorText().c_str());
    } else {
        printf("Hello returned: %s\n",resp.hello_msg().c_str());
    }
}

int main(int argc, char**argv)
{
    struct sockaddr_in sa;
    int port;
    int s;

    int argindex = parse_options(argc,argv);
    if(argindex>argc-1) {
        usage(argv[0]);
        exit(1);
    }
    port = atoi(argv[argindex]);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(0 > (s = socket(PF_INET,SOCK_STREAM,0))) {
        perror("socket");
        exit(1);
    }

    if(0 != connect(s,(struct sockaddr*)&sa,sizeof(struct sockaddr_in))) {
        perror("connect");
        exit(1);
    }

    // Get a new Connection that uses this socket
    trpc::Connection * cconn = new trpc::Connection(s);

    // Set up the RPC stub
    trpc::SocketRpcChannel channel(cconn);
    trpc::hellotest::HelloService_Stub hs(&channel);

    // launch a worker thread that runs the connection
    ClientThread client(cconn); 
    pthread_t cthread;
    int ret = pthread_create(&cthread,NULL,start_thread,&client);
    if(ret != 0) {
        fprintf(stderr,"pthread_create failed: %d (%s)\n",ret,strerror(ret));
        exit(1);
    }

    run_tests(hs);

    // whack the socket
    shutdown(s,SHUT_RDWR);
    close(s);

    // thread join  & exit
    pthread_join(cthread,NULL);

    delete cconn;

    return 0;
}
