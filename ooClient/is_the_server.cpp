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

    int old_opt = setFdNonBlocking(sockClient.getFd());
    if (old_opt == -1) {
        printf("fcntl failed --- \n");
        return -1;
    }
    int flag = connect(sockClient.getFd(), (sockaddr*) &conAddr, sizeof(sockaddr_in));
    if (flag == 0) {
        fcntl(sockClient.getFd(), F_SETFL, old_opt);
        return 0;
    }
    else if (errno != EINPROGRESS) {
        return -1;
    }

    fd_set set;
    timeval tm;
    tm.tv_sec = timeout;
    tm.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(sockClient.getFd(), &set);
    if (select(sockClient.getFd() + 1, NULL, &set, NULL, &tm) > 0) {
        int error;
        socklen_t len = sizeof(error);
        getsockopt(sockClient.getFd(), SOL_SOCKET, SO_ERROR, &error, &len);
        if (!error)
            printf("is the ip %s -- port %d \n", conIP, conPort);
        else
            ret = -1;
    }
    else
        ret = -1;

    sockClient.closeFd();
    return ret;
}
