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


int main(int argc, char* argv[])
{
    int64_t start=nowTime();
    if (argc < 2) {
        std::cerr << "argc < 2   !\n ";
        return -1;
    }
    bool buffering = false;
    bool nodelay=false  ;
    for(int i= 1;i< argc;++i)
    {
        switch (*argv[i])
        {
        case 'B':
            buffering=true;
            break;
        case 'D':
            nodelay=true;
            break;
            break;
        default:
            break;
        }
    }
    net::TcpSocket sock;
    sock.CreateSocket();
    char* fromIP = argv[1];
    UINT fromPort = kPort;
    if (!sock.Connect(fromIP, fromPort)) {
        printErrorMsg("connect");
    }
    SessionInfo session_info={8196,0};
    size_t payload_size=session_info.length+sizeof(int32_t);
    PayloadInfo* payload_info= static_cast<PayloadInfo*>(::malloc(payload_size));
    payload_info->length=session_info.length;
    if(nodelay)
    {
       net::TcpSocket(sock.getFd());
    }
    if(buffering)
    {
        std::string buf((sizeof session_info+payload_size),0);
        memcpy(const_cast<char*>(buf.data()),&session_info,sizeof session_info);
        memcpy(const_cast<char*>(buf.data())+sizeof session_info,payload_info,payload_size);
        sock.write_n(buf.data(),buf.size());
    }
    else
    {
        sock.write_n(&session_info,sizeof session_info);
        sock.write_n(payload_info,payload_size);
    }

    int ack;
    sock.Receive(&ack,sizeof ack);
    int64_t end=nowTime();
    std::cout << "passing time : "<<end-start<<std::endl;
    ::free(payload_info);
    sock.closeFd();
    return 0;

}

