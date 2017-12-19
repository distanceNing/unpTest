//
// Created by yangning on 17-12-2.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include "socket/epoll_test.h"
#include "socket/tcp_socket.h"
#include "thread/thread_pool.h"
#include "common.h"
static int gUserCount = 0;

void clientRcb(int fd, Epoll& epoll);

static int concurrent_num = 20;

static bool isgLoop = true;
int main(int argc, char* argv[])
{
    //if(argc < 2)
    //{
    //    printf("Usage: %s [host] [port] [connection_num] \n",argv[0]);
    //    return 0;
    //}
    const char* server_ip = "127.0.0.1"; //argv[1]

    ThreadPool::ThreadTask task;
    task = [server_ip](){
      Epoll epoll;
      for (int i = 0; i < concurrent_num; ++i)
      {
          usleep(1000);
          int conn_fd;
          if ( (conn_fd=net::TcpSocket::create_and_bind())<0)
              printErrorMsg("creat socket");
          if ( !net::TcpSocket::sockConnect(conn_fd,server_ip,kPort))
              printErrorMsg("connect");
          printf("connect times is %d \n",i);
          Epoll::EventCallBack fun = std::bind(clientRcb, conn_fd, epoll);
          epoll.addNewFd(conn_fd, fun);
      }
      while (isgLoop)
          epoll.epollWait();

    };
    ThreadPool threadPool(task,10);
    threadPool.run();
    return 0;
}

void clientRcb(int fd, Epoll& epoll)
{
    char recv_buf[MAX_BUF_SIZE] = {'\0'};
    ssize_t read_size = read(fd, recv_buf, MAX_BUF_SIZE);
    if ( read_size == 0 ) {
        epoll.removeFd(fd);
        isgLoop = false;
        close(fd);
        std::cout << "client fd " << fd << " close --\n";
    }
    else if ( read_size > 0 ) {
        assert(!strncmp(recv_buf,"hello world",11));
        ::write(fd,recv_buf,(size_t)read_size);
    }
    else {
        printErrorMsg("read");
    }
}

