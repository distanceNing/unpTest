//
// Created by yangning on 17-10-18.
//

#include "epoll_test.h"
#include "common.h"

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

ERROR_TYPE epollHandleConnect(int listenFd)
{
    struct epoll_event event;
    setFdNonBlocking(listenFd);
    char connIP[32] = {'\0'};
    int epoll_fd = epoll_create1(EPOLL_FLAGS);
    if (epoll_fd<0)
        printErrorMsg("epoll create");

    event.data.fd = listenFd;
    event.events = EPOLLIN | EPOLLET;
    int flag = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenFd, &event);

    if (flag<0)
        printErrorMsg("epoll _ctl");
    EpollEventList epollEventList(INIT_SIZE);
    std::vector<int> clientFdList;
    char recv_buf[BUF_SIZE];
    while (true) {
        int ready_num = epoll_wait(epoll_fd, epollEventList.data(), epollEventList.size(), MAYBE_TIME_OUT);
        if (ready_num==0) {
            std::cout << "No Event Happened --- \n";
            continue;
        }
        //当返回的准备好的事件和pollEventList的长度相同时,进行扩充.
        if(epollEventList.size() == ready_num)
        {
            epollEventList.resize(epollEventList.size()*2);
        }
        //处理文件描述符上发生的事件
        for (int i = 0; i<ready_num; ++i)
        {
            if (epollEventList[i].data.fd==listenFd)
            {
                struct sockaddr_in cliaddr;
                __socklen_t clilen = sizeof(cliaddr);
                int connfd;
                if ((connfd = accept(listenFd, (struct sockaddr*) &cliaddr, &clilen))<0) {
                    perror("accept error");
                    return -1;
                }
                memset(connIP, '\0', 32);
                strcpy(connIP, inet_ntoa(cliaddr.sin_addr));
                printf("connect IP: %s ------ Port: %d\n", connIP, ntohs(cliaddr.sin_port));
                //将新连接的文件描述符加入到pollArray
                clientFdList.push_back(connfd);
                setFdNonBlocking(connfd);
                event.data.fd = connfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event);
            }
            else
            {
                memset(recv_buf, 0, BUF_SIZE);
                size_t read_size = read(epollEventList[i].data.fd, recv_buf, BUF_SIZE);
                if (read_size==0)
                {
                    std::cout << "client fd " << epollEventList[i].data.fd << " close --\n";
                }
                else if (read_size>0)
                {
                    write(epollEventList[i].data.fd, recv_buf, strlen(recv_buf));
                }
                else
                {
                    printErrorMsg("read");
                }
            }

        }

    }

    return 0;
}
