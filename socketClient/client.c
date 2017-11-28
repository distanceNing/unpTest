#define SERVER_PORT 9000

#include "strClient.h" 
static const char* kServerIP="127.0.0.1";
int main(int argc,char* argv[])
{
    int i,connfd;

    struct sockaddr_in conAddr;
    
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(connfd<0)
    {
        printf(" %s\n",strerror(errno));
        return -1;
    }
    memset(&conAddr,0,sizeof(conAddr));
    conAddr.sin_family= AF_INET ;
    conAddr.sin_addr.s_addr = inet_addr(kServerIP);
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


