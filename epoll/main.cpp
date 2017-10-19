#include "epoll_test.h"
#include "tcp_socket.h"
#include "common.h"

int main()
{
    TcpSocket listenSock;
    listenSock.CreateSocket(AF_INET,SOCK_STREAM,kPort);
    setFdNonBlocking(listenSock.GetSocket());
    listenSock.Listen();

    epollHandleConnect(listenSock.GetSocket());

    return 0;
}