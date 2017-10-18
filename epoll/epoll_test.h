//
// Created by yangning on 17-10-18.
//

#ifndef EPOLL_EPOLL_TEST_H
#define EPOLL_EPOLL_TEST_H

#include <sys/epoll.h>

#include <vector>

#define INIT_SIZE 16

#define BUF_SIZE 1024

#define MAYBE_TIME_OUT -1

#define EPOLL_FLAGS 0

typedef int ERROR_TYPE;

using EpollEventList=std::vector<struct epoll_event>;

ERROR_TYPE epollHandleConnect( int listenFd);

#endif //EPOLL_EPOLL_TEST_H
