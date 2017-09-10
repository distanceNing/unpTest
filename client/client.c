#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#define SERVER_PORT 9000
#include "strClient.h" 
int main(int argc,char* argv[])
{
    int connfd;
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(argc!=2)
    {
       printf("argc !=  2\n");
       return -1;
    }
    if(connfd<0)
    {
        printf(" %s\n",strerror(errno));
        return -1;
    }
/*   
 *  memset(&serveraddr,0,sizeof(struct sockaddr_in));
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    inet_aton(argv[1], &serveraddr.sin_addr);  
*/  
    char* conIP=argv[1];

    struct sockaddr_in conAddr = { AF_INET };
    conAddr.sin_addr.s_addr = inet_addr(conIP);
    conAddr.sin_port = htons(SERVER_PORT);
    socklen_t len = sizeof(conAddr);

    
    if(connect(connfd, (struct sockaddr*)&conAddr,len)!=0)
    {
        printf(" %s\n",strerror(errno));
        return -1;
    }

    strClient(stdin,connfd);
    


    return 0;
}


