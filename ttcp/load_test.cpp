//
// Created by yangning on 17-12-1.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include <iostream>
#include "socket/tcp_socket.h"
#include "common.h"
int main()
{
    net::TcpSocket sock;
    if (!sock.CreateSocket(kPort))
        printErrorMsg("creat socket");
    sock.Listen();
    std::cout << "wait for connection \n";
    char conn_ip[32];
    UINT conn_port;
    int conn_fd = -1;
    ssize_t read_size;
    ssize_t total_size;

    while (true) {
        memset(conn_ip, '\0', 32);
        if ((conn_fd = sock.Accept(conn_ip, conn_port)) < 0) {
            printErrorMsg("accept");
        }
        printf("connect IP: %s ------ Port: %d\n", conn_ip, conn_port);

        net::TcpSocket client(conn_fd);
        char buffer[MAX_BUF_SIZE]={'\0'};
        int64_t start,end;
        total_size=0;
        ::sleep(1);
        start=nowTime();
        while(true)
        {
            read_size=client.Receive(buffer,MAX_BUF_SIZE);
            end=nowTime();
            if(read_size > 0) {
                total_size += read_size;
                if ((end - start) >= 1000 * 1000 )
                {
                    start=end;
                    std::cout << total_size / (1024 * 1024) << " MB/s ." << std::endl;
                    total_size=0;
                }
                continue;
            }
            else if(read_size == 0)
                break;
            else
                printErrorMsg("recv");
        }

        client.closeFd();
    }
    return 0;
}