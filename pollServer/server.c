#include <stdlib.h>
#include <signal.h>
#define SERVER_PORT 9000
#include "strEcho.h" 
#include <time.h>
int main()
{
    int recv_size=-1;
    time_t start_time=time(0);
    struct tm* tm=localtime(&start_time);
    printf("server start time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);
    int i,connfd,maxi,listenfd,maxfds,nready;
    int flag;
    struct pollfd clientfd[OPEN_MAX];
    socklen_t clilen;
//    struct pollfd* clientfd;
    char connIP[32];
    char recv_buf[BUF_SIZE];
    struct sockaddr_in cliaddr,serveraddr;
    
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    
    memset(&serveraddr,0,sizeof(struct sockaddr_in));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serveraddr.sin_port=htons(SERVER_PORT);
    
    bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(listenfd,5);


//    clientfd=initPollArray(OPEN_MAX);
//    if(clientfd==NULL)
//    {
//        printf("initPollArray failed !\n");
//        return -1;
//    }
//    printf("initPollArray success ! \n");
//    flag=pollHandleConnect(clientfd,listenfd,OPEN_MAX);
//    free(clientfd);

    printf("wait for connect \n");
    maxi=0;
    clientfd[0].fd=listenfd;
    clientfd[0].events=POLLIN;
    for(i=1;i<OPEN_MAX;++i)
        clientfd[i].fd=-1;

    for(;;)
    {
        nready=poll(clientfd,maxi+1,-1);
        if(nready==-1)
        {
            perror("poll error: ") ;
            return -1;  
        }
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
            for(i=1;i<OPEN_MAX;++i)
            {
                if(clientfd[i].fd==-1)
                {
                    clientfd[i].fd=connfd;
                    break;
                }
            }
            if(i==OPEN_MAX)
            {
                perror("too many clients");
                return -1;
            }
            clientfd[i].events=POLLIN;
            maxi=maxi > i ? maxi : i; 
            if(--nready<=0)
                continue;
        }
        for(i=1;i<=maxi;i++)
            {
                memset(recv_buf,BUF_SIZE,0);
                if(clientfd[i].fd<0)
                    continue;
                if(clientfd[i].revents&POLLIN)
                {
                    printf("fd %d is readable\n",clientfd[i].fd);
                    if((recv_size=read(clientfd[i].fd,recv_buf,BUF_SIZE))==0)
                    {
                        close(clientfd[i].fd);
                        clientfd[i].fd=-1;
                    }
                    else
                        write(clientfd[i].fd,recv_buf,recv_size);
                if(--nready<=0)
                    break;
                }
            }
    }
    
    time_t end_time=time(0);
    tm=localtime(&end_time);
    printf("server start time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);
    return 0;
}


