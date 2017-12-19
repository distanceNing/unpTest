//
// Created by yangning on 17-11-30.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include <cstring>
#include <iostream>
#include "common.h"

int main()
{
    int sock=::socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);
    server_addr.sin_port=htons(kPort);
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    socklen_t len=sizeof server_addr;
    int i=0;
    char buffer[MAX_BUF_SIZE];
    while (true) {
        memset(buffer,0,MAX_BUF_SIZE);
        scanf("%s",buffer);
        ssize_t send_size = ::sendto(sock, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &server_addr, len);
        if (send_size < 0)
            printErrorMsg("sendto");
        memset(buffer,0,MAX_BUF_SIZE);
        ssize_t recv_size = ::recvfrom(sock,buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &server_addr, &len);
        if (recv_size < 0)
            printErrorMsg("recvfrom");
        else if (recv_size == 0)
        {
            printf("server closed \n");
            break;
        }
        else
            std::cout<<buffer<<std::endl;
    }
    return 0;
}

