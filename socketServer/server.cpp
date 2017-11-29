#include <stdlib.h>
#include <signal.h>
#define SERVER_PORT 9000
#include "strEcho.h"
int main()
{
    ssize_t recv_size = -1;
    int i, connfd, sockfd, maxi, listenfd, clientfd[FD_SETSIZE], maxfds, nready;

    socklen_t clilen;
    char connIP[32];
    char recv_buf[BUF_SIZE];
    struct sockaddr_in cliaddr, serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serveraddr.sin_port = htons(SERVER_PORT);
    bind(listenfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
    listen(listenfd, 5);

    printf("wait for connect \n");
    maxi = 0;
    maxfds = listenfd;
    fd_set allset, rset;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    FD_SET(STDIN_FILENO,&allset);
    for (i = 0; i < FD_SETSIZE; ++i)
        clientfd[i] = -1;
    for (;;)
    {
        clilen = sizeof(cliaddr);
        //重置fdset
        rset = allset;
        nready = select(maxfds + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset)) {
            if ((connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen)) < 0) {
                perror("accept");
                return -1;
            }
            memset(connIP, '\0', 32);
            strcpy(connIP, inet_ntoa(cliaddr.sin_addr));
            printf("connect IP: %s Port: %d\n", connIP, ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; ++i) {
                if (clientfd[i] == -1) {
                    clientfd[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                perror("too many clients");
                return -1;
            }
            FD_SET(connfd, &allset);
            if (connfd > maxfds)
                maxfds = connfd;
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        if(FD_ISSET(STDIN_FILENO, &rset))
        {
            memset(recv_buf, 0, BUF_SIZE);
            recv_size = read(STDIN_FILENO, recv_buf, BUF_SIZE);
            for(int j = 0; j<= maxi;++j)
            {
                if(clientfd[j]>0)
                {
                    write(clientfd[j], recv_buf, (size_t)recv_size);
                }
            }
        }
        for (i = 0; i <= maxi; i++) {

            if ((sockfd = clientfd[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                memset(recv_buf, 0, BUF_SIZE);
                if ((recv_size = read(sockfd, recv_buf, BUF_SIZE)) == 0) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    clientfd[i] = -1;
                }
                else
                    printf("%d send : %s \n",sockfd, recv_buf);
            }
            if (--nready < 0)
                break;
        }
    }

    return 0;
}


