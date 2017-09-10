#include "strEcho.h"

bool clientHandle(int clientfd)
{
    int recv_size=-1;
    char recv_buf[BUF_SIZE];
    memset(recv_buf,0,BUF_SIZE);
    if((recv_size=read(clientfd,recv_buf,BUF_SIZE))==0)
        return false;
    write(clientfd,recv_buf,recv_size);
    return true;
}

struct pollfd* initPollArray(int openMax)
{
    int i;
    struct pollfd* clientfd=(struct pollfd*)malloc(openMax*sizeof(struct pollfd));
    for(i=1;i<openMax;++i)
       clientfd[i].fd=-1;
    return clientfd;
}

ERROR_TYPE pollHandleConnect(struct pollfd* clientfd,int listenfd,int openMax)
{
    int i,nready,maxi;
    int connfd;
    char connIP[32];
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    memset(&cliaddr,0,sizeof(cliaddr));
    clientfd[0].fd=listenfd;
    clientfd[0].events=POLLIN;
    maxi=0;

    printf("wait for connection ! \n");
    for(;;)
    {
        nready=poll(clientfd,maxi,-1);
        if(nready==-1)
        {
            perror("poll error;");
            return -1;
        }
        //判断监听描述符是否可读
        if(clientfd[0].revents&POLLIN)
        {
            clilen=sizeof(cliaddr);
            if((connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen))<0)
            {
                perror("accept error");
                return -1;
            }
            memset(connIP,'\0',32);
            strcpy(connIP,inet_ntoa(cliaddr.sin_addr));
            printf("connect IP: %s Port: %d\n",connIP,ntohs(cliaddr.sin_port));
            //将新连接的文件描述符加入到pollArray
            for(i=1;i<OPEN_MAX;++i)
            {
                if(clientfd[i].fd==-1)
                {
                    clientfd[i].fd=connfd;
                    break;
                }
            }
            if(i==openMax)
            {
                perror("too mang client");
                return -1;
            }
            maxi=maxi > i ? maxi : i;
            clientfd[i].events=POLLIN;
            if(--nready<=0)
                continue;
        }
        //处理可读的clientfd
        for(i=1;i<maxi;++i)
        {
            if(clientfd[i].fd==-1)
                continue;
            if(clientfd[i].events&POLLIN)
            {
                if(!clientHandle(clientfd[i].fd))
                {
                    close(clientfd[i].fd);
                    clientfd[i].fd=-1;
                }
            }
            if(--nready<=0)
                break;
        }
    }
}
