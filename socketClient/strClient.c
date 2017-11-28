#include "strClient.h"
int max(int value1, int value2)
{
    if (value1 > value2)
        return value1;
    return value2;
}

void strClient(FILE* fp, int sock)
{

    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];

    int maxfd;
    ssize_t size = -1;
    fd_set rset;
    FD_ZERO(&rset);
    for (;;) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sock, &rset);
        maxfd = max(fileno(fp), sock) + 1;
        memset(recv_buf, 0, BUF_SIZE);
        select(maxfd, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sock, &rset)) {
            if (read(sock, recv_buf, BUF_SIZE) < 0) {
                perror("server no ret");
                return;
            }
            fputs(recv_buf, stdout);
        }
        if (FD_ISSET(fileno(fp), &rset)) {
            if (fgets(send_buf, BUF_SIZE, fp) == NULL)
                return;
            size = write(sock, send_buf, strlen(send_buf));
            printf("write size %d\n", (int) size);
        }
    }

}
