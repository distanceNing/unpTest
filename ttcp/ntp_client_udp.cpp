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
    NTPInfo ntp_info={nowTime(),0};

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);
    server_addr.sin_port=htons(kPort);
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    socklen_t len=sizeof server_addr;
    int i=0;
    int64_t pass_time=0;
    while (true) {
        ssize_t send_size = ::sendto(sock, &ntp_info, sizeof ntp_info, 0, (struct sockaddr*) &server_addr, len);
        if (send_size < 0)
            printErrorMsg("sendto");
        ssize_t recv_size = ::recvfrom(sock, &ntp_info, sizeof ntp_info, 0, (struct sockaddr*) &server_addr, &len);
        if (recv_size < 0)
            printErrorMsg("recvfrom");
        else if (recv_size == 0)
        {
            printf("server closed \n");
            break;
        }
        else if(recv_size== sizeof ntp_info)
        {
            int64_t now=nowTime();
            std::cout<<"now time :"<<now;
            std::cout << " pass time :" << now-ntp_info.response<< std::endl;
            pass_time += now-ntp_info.response;
        }

        if(++i == 50)
            std::cout<<"50 average clock error : "<<pass_time/50<<std::endl;
        ::usleep(200*1000);
    }
    return 0;
}