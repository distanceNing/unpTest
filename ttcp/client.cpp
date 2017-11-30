//
// Created by yangning on 17-11-30.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include <iostream>
#include "socket/tcp_socket.h"
#include "common.h"
#include "time_stamp.h"
int main(int argc, char** argv)
{
    net::TimeStamp start_time;
    start_time.printTime();
    if (argc < 2) {
        std::cout << "need server ip , buffer length ,loop number\n";
        return EXIT_FAILURE;
    }
    SessionInfo session_info = {0, 0};
    session_info.number = atoi(argv[2]);
    session_info.length = atoi(argv[3]);
    std::cout << "buffer length is " << session_info.length << "  loop number is " << session_info.number << std::endl;
    int64_t total_size = session_info.length * session_info.number;
    std::cout << "total need send size is " << total_size / (1024 * 1024) << " MB ." << std::endl;

    net::TcpSocket sock;
    if (!sock.CreateSocket()) {
        printErrorMsg("creat socket");
    }
    if (!sock.Connect(argv[1], kPort)) {
        printErrorMsg("connect");
    }

    sock.Send(&session_info, sizeof(session_info));
    size_t payload_size = (size_t) session_info.length + sizeof(int32_t);
    PayloadInfo* payload_info = static_cast<PayloadInfo*>(malloc(payload_size));
    payload_info->length = session_info.length;
    ssize_t send_size;
    size_t a_send_size;
    int ack = 0;
    size_t total_send_size = 0;
    for (int i = 0; i < session_info.number; ++i)
    {
        if(sock.write_n(payload_info,payload_size)!=payload_size)
        {
            printErrorMsg("short write");
        }

        if(sock.Receive(&ack, sizeof(ack))!=sizeof(ack))
        {
            printErrorMsg("recv");
        }
        assert(ack == session_info.length);

        total_send_size += send_size;
    }
    std::cout << "send total size is " << total_send_size / (1024 * 1024) << " MB ." << std::endl;
    net::TimeStamp end_time;
    end_time.printTime();
    time_t total_time = end_time - start_time;
    std::cout << "total time " << total_time << std::endl;

    return 0;
}