//
// Created by yangning on 17-12-2.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include "socket/epoller.h"
#include "socket/tcp_socket.h"
#include "thread/thread_pool.h"
#include "common.h"
#include "benckmark.h"
static int gUserCount = 0;

static int concurrent_num = 1024;

const int bufsize = 1024;

static bool isgLoop = true;

const int timeout = 1;

typedef struct {
  const char* server_ip_;
  int server_port_;
  bool is_keep_alive_;
  size_t thread_num_;
  size_t concurrent_num_;
  size_t connect_timeout_ms_;
}Option;

Option gOption;

void defautOption()
{
    gOption.server_ip_=  "127.0.0.1";
    gOption.connect_timeout_ms_ = 3000;
    gOption.server_port_ = 9000;
    gOption.concurrent_num_ =1024;
    gOption.is_keep_alive_ = true;
    gOption.thread_num_ = 1;
}

int main(int argc, char* argv[])
{
    //if(argc < 2)
    //{
    //    printf("Usage: %s [host] [port] [connection_num] \n",argv[0]);
    //    return 0;
    //}

    defautOption();
    ThreadPool::ThreadTask task;

    task = []() {
      Epoll epoll;
      epoll.setReadcb([](net::SocketBuf* buf,int fd){
        //printf("%s\n",buf->readBegin());
        char buffer[MAX_BUF_SIZE]={'\0'};
        size_t readable=buf->readableBytes();
        if(buf->read(buffer,readable))
            ::write(fd,buffer,readable);
      });

      for (int i = 0; i < concurrent_num; ++i)
      {
          int conn_fd ;
          if ((conn_fd = net::TcpSocket::create_and_bind()) < 0 )
              printErrorMsg("creat socket");
          if ( !setFdNonBlocking(conn_fd))
              printErrorMsg("setFdNonBlocking");
          if ( net::TcpSocket::noblockingConnect(conn_fd, gOption.server_ip_, kPort,gOption.connect_timeout_ms_) < 0 ) {
              //printf("connect time out\n");
              printErrorMsg("connect");
          }
          
          printf("connect time is %d \n", i);
          net::TcpSocket::setTcpNoDelay(conn_fd);
          //::setsockopt(conn_fd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
          //::setsockopt(conn_fd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
          epoll.addNewFd(conn_fd);
          usleep(1000);
      }
      while (isgLoop)
          epoll.epollWait();

    };


    ThreadPool threadPool(task, gOption.thread_num_);
    threadPool.run();

    threadPool.join();
    return 0;
}

