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

    while (true) {
        memset(conn_ip, '\0', 32);

        if ((conn_fd = sock.Accept(conn_ip, conn_port)) < 0) {
            printErrorMsg("accept");
        }
        printf("connect IP: %s ------ Port: %d\n", conn_ip, conn_port);

        net::TcpSocket client(conn_fd);
        SessionInfo session_info = {0, 0};
        client.Receive(&session_info, sizeof(session_info));

        std::cout << "buffer length is " << session_info.length << "  loop number is " << session_info.number
                  << std::endl;
        size_t payload_size = (size_t) session_info.length + sizeof(int32_t);
        PayloadInfo* payload_info = static_cast<PayloadInfo*>(::malloc(payload_size));
        assert(payload_info != NULL);

        int ack = session_info.length;
        if (client.read_n(payload_info, payload_size) != payload_size) {
            printErrorMsg("short read");
        }
        assert(payload_info->length == session_info.length);
        if (client.write_n(&ack, sizeof(ack)) != sizeof(ack)) {
            printErrorMsg("write");
        }
        ::free(payload_info);
        client.closeFd();
    }
    return 0;
}

