#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SERVER_PORT 9000
#include "strEcho.h" 
int main()
{
    int listenfd,connfd;
    pid_t childpid;
    socklen_t clilen;
    char connIP[32];
    struct sockaddr_in cliaddr,serveraddr;
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&serveraddr,0,sizeof(struct sockaddr_in));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    serveraddr.sin_port=htons(SERVER_PORT);
    bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(listenfd,5);
    printf("wait for connect \n");
    for(;;)
    {
        clilen=sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
        if((childpid=fork())==0)
            {
                close(listenfd);
                memset(connIP,'\0',32);
                strcpy(connIP,inet_ntoa(cliaddr.sin_addr));
                printf("IP %s\n",connIP);
                strEcho(connfd);
                exit(0);
            }
    }
    


    return 0;
}


