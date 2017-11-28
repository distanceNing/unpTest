#include "epoll_test.h"
#include "tcp_socket.h"
#include "common.h"

void listenRcb(int listenFd, Epoll& epoll);

void clientRcb(int fd,Epoll& epoll );
int main()
{
    SocketFd listenSock;
    listenSock.CreateSocket(kPort);
    listenSock.Listen();

    Epoll epoll;
    Epoll::EventCallBack fun=std::bind(listenRcb,listenSock.getFd(),epoll);
    epoll.addNewFd(listenSock.getFd(),fun);
    while (true)
        epoll.epollWait();
    return 0;
}

void clientRcb(int fd,Epoll epoll)
{
    char recv_buf[BUF_SIZE];
    memset(recv_buf, 0, BUF_SIZE);
    ssize_t read_size=0;
    ssize_t size;
    while((size = read(fd, recv_buf, BUF_SIZE)) > 0 )
    {
        read_size += size;
    }

    if (size == 0)
    {
        epoll.removeFd(fd);
        //关闭client socket
        close(fd);
        std::cout << "client fd " << fd << " close --\n";
    }
    else if (read_size > 0)
    {
        write(fd, recv_buf, strlen(recv_buf));
    }
    else
    {
        printErrorMsg("read");
    }
}


void listenRcb(int listenFd, Epoll epoll)
{
    char recv_buf[BUF_SIZE] = {'\0'};
    char connIP[32] = {'\0'};
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd;
    if ((connfd = accept(listenFd, (struct sockaddr*) &cliaddr, &clilen)) < 0)
    {
        printErrorMsg("accept");
    }

    memset(connIP, '\0', 32);
    strcpy(connIP, inet_ntoa(cliaddr.sin_addr));
    printf("connect IP: %s ------ Port: %d\n", connIP, ntohs(cliaddr.sin_port));

    //将新连接的文件描述符加入到clienArray
    Epoll::EventCallBack fun=std::bind(clientRcb,connfd,epoll);
    epoll.addNewFd(connfd,fun);
}