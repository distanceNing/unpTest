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
    char send_msg[256];
    net::SocketBuf buf;
    net::SocketBuf read_buf;
    int64_t start_time=nowTime();
    for (int i=0;i<1024;++i)
    {
        memset(send_msg,0,256);
        sprintf(send_msg,"set yn%d 1 1 1024\r\n",i);
        std::string data(1024,'a');
        data+="\r\n";
        buf.append(send_msg,strlen(send_msg));
        buf.append(data);
        if((send_size=sock.Send(buf))>0)
        {
            printf("send time is %d send size is %d \n",i,(int)send_size);
            if(send_size < buf.readableBytes())
            {
                printf("short write -- \n");
            }
        }
        else if(send_size == 0)
            printf("peer sock closed\n");
        else if(errno ==EAGAIN)
            printf("error eagain\n");
        else
            printErrorMsg("send");
        buf.resetBuffer();
        sock.Receive(read_buf);
        printf("%s",read_buf.readBegin());
        read_buf.resetBuffer();
        ::usleep(1000);
    }

    int64_t end_time=nowTime();
    time_t total_time = end_time - start_time;
    std::cout << "total time " << total_time/(1000) << std::endl;

    return 0;
}


