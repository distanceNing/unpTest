
#include "strClient.h"
void strClient(FILE* fp,int sock)
{

    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];
    int size=-1;
    while(fgets(send_buf,BUF_SIZE,fp)!=NULL)
    {
        memset(recv_buf,0,BUF_SIZE);
        size=write(sock,send_buf,strlen(send_buf));
        printf("write size %d\n",size);
        if(read(sock,recv_buf,BUF_SIZE)<0)
            perror("server no ret");
        fputs(recv_buf,stdout);
    }

}
