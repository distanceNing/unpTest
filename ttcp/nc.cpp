//
// Created by yangning on 17-11-30.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include <iostream>
#include <csignal>
#include "socket/tcp_socket.h"
#include "common.h"
#include "time_stamp.h"
int main(int argc, char** argv)
{
    int64_t start_time=nowTime();
    const char* server_ip="127.0.0.1";
    net::TcpSocket sock;
    if (!sock.CreateSocket()) {
        printErrorMsg("creat socket");
    }
    if (!sock.Connect(server_ip, kPort)) {
        printErrorMsg("connect");
    }

    ssize_t send_size;
    size_t total_send_size = 0;
    setFdNonBlocking(sock.getFd());
    for (int i=0;;++i)
    {

        std::string send(65536,'s');
        if((send_size=sock.Send(send.data(),send.size()))>0)
        {
            printf("send time is %d send size is %d\n",i,send_size);
            if(send_size != send.size())
            {
                printf("short write -- \n");
                ::usleep(1000*100);
            }
        }
        else if(send_size == 0)
            printf("peer sock closed\n");
        else if(errno ==EAGAIN)
            printf("error eagain\n");
        else
            printErrorMsg("send");
    }

    int64_t end_time=nowTime();
    time_t total_time = end_time - start_time;
    std::cout << "total time " << total_time << std::endl;

    return 0;
}


