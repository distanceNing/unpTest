#include "common.h"
#include "socket/tcp_socket.h"

#include <iostream>
#include <unistd.h>
#include <memory>
ssize_t receive_file(const char* filename, net::TcpSocket& sock);
int main()
{
    net::TcpSocket sock;
    if (!sock.CreateSocket(kPort))
        printErrorMsg("socket");
    sock.Listen();
    char conn_ip[32];
    UINT conn_port;
    int conn_fd = -1;
    char file_name[256];
    const char* hello = "hello world";
    while (true) {
        std::cout << "wait for connection!\n";

        if ((conn_fd = sock.Accept(conn_ip, conn_port)) < 0) {
            printErrorMsg("accept");
        }
        printf("connect IP: %s ------ Port: %d\n", conn_ip, conn_port);

        net::TcpSocket client(conn_fd);
        memset(file_name, 0, 256);
        ssize_t size = client.Receive(file_name, 256);
        int ack = 0;
        client.Send(&ack, sizeof ack);
        client.closeFd();
        client.Send(hello,strlen(hello));
        ssize_t recv_size = receive_file(file_name, client);
        std::cout << "recv size :" << recv_size << std::endl;
        std::cout << "copy successful: " << file_name << std::endl;

    }
    return 0;
}
ssize_t receive_file(const char* filename, net::TcpSocket& sock)
{

    FILE* fp = fopen(filename, "wb");
    char buffer[MAX_BUF_SIZE];
    memset(buffer, '\0', MAX_BUF_SIZE);
    ssize_t recv_size = 0;
    ssize_t size;
    while ((size = sock.Receive(buffer, MAX_BUF_SIZE)) > 0) {
        fwrite(buffer, 1, (size_t) size, fp);
        recv_size += size;
    }
    return recv_size;
}