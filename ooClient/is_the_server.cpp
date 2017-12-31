#include "is_the_server.h"
#include "tcp_socket.h"
#include "common.h"
int IsServerIP(const char* conIP, const int conPort, const int timeout)
{
    int ret = 0;
    net::SocketFd sockClient;
    sockClient.CreateSocket();
    sockaddr_in conAddr = {AF_INET};
    conAddr.sin_addr.s_addr = inet_addr(conIP);
    conAddr.sin_port = htons(static_cast<uint16_t>(conPort));

    if ( setFdNonBlocking(sockClient.getFd()) < 0 ) {
        printf("fcntl failed --- \n");
        return -1;
    }
    int flag = connect(sockClient.getFd(), (sockaddr*) &conAddr, sizeof(sockaddr_in));
    if ( flag == 0 ) {
        //fcntl(sockClient.getFd(), F_SETFL, old_opt);
        return 0;
    }
    else if ( errno != EINPROGRESS ) {
        return -1;
    }
    int fd = sockClient.getFd();
    fd_set rset, wset;
    timeval tm;

    tm.tv_sec = timeout;
    tm.tv_usec = 0;
    FD_ZERO(&rset);
    wset = rset;
    FD_SET(fd, &rset);
    FD_SET(fd, &wset);
    int nready = select(fd + 1, &rset, &wset, NULL, &tm);
    if ( nready == 0 ) {
        errno = ETIMEDOUT;
        return -1;
    }
    else if ( FD_ISSET(fd, &wset) || FD_ISSET(fd, &rset)) {
        int error;
        socklen_t len = sizeof(error);
        getsockopt(sockClient.getFd(), SOL_SOCKET, SO_ERROR, &error, &len);
        if ( !error )
            printf("is the ip %s -- port %d \n", conIP, conPort);
        else {
            perror("nonblocking connect");
            ret = -1;
        }
    }
    else
        printErrorMsg("select");
    sockClient.closeFd();
    return ret;
}
