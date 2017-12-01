//
// Created by yangning on 17-11-30.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//
#include <cstring>
#include "common.h"
int main()
{
    int sock=::socket(AF_INET,SOCK_DGRAM,0);

    struct sockaddr_in local_addr;
    memset(&local_addr,0,sizeof local_addr);
    local_addr.sin_family=AF_INET;
    local_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    local_addr.sin_port=::htons(kPort);
    if(::bind(sock,(struct sockaddr*)&local_addr,sizeof local_addr)<0)
        printErrorMsg("bind");

    while (true)
    {
        NTPInfo ntp_info;
        struct  sockaddr_in client_addr;
        socklen_t socklen=sizeof client_addr;
        ssize_t recv_size=::recvfrom(sock,&ntp_info,sizeof ntp_info,0,(struct sockaddr*)&client_addr,&socklen);
        printf("recv from ip: %s port: %d\n",inet_ntoa(client_addr.sin_addr),htons(client_addr.sin_port));
        if(recv_size == sizeof ntp_info)
        {
            ntp_info.response=nowTime();
            ::sendto(sock,&ntp_info,sizeof ntp_info,0,(struct sockaddr*)&client_addr,socklen);
        }
        else if(recv_size == 0)
            printf("connection closed \n");
        else
            printErrorMsg("recv from");

    }
    return 0;
}
