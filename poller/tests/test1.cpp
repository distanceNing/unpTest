#include "print_time.h"
#include "event_loop.h"
#include "tcp_socket.h"

const int kPort = 8000;

void wcb(int fd)
{
    char buffer[]="hello world";
    size_t size=write(fd,buffer,strlen(buffer));
    printf("writeable write size is : %lu\n",size);
}

void rcb(int listenfd)
{
    int connfd=-1;
    char connIP[32];
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    clilen=sizeof(cliaddr);
    memset(&cliaddr,0,sizeof(cliaddr));
    if((connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen))<0)
    {
        perror("accept error");
    }
    memset(connIP,'\0',32);
    strcpy(connIP,inet_ntoa(cliaddr.sin_addr));
    printf("connect IP: %s ------ Port: %d\n",connIP,ntohs(cliaddr.sin_port));
}

int main()
{
    time_t start_time=time(0);
    struct tm* tm=localtime(&start_time);
    printf("server start time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);

    //建立监听socket
    Csocket server_sock;
    server_sock.CreatSocket(AF_INET,SOCK_STREAM,kPort);
    server_sock.Listen();

    Channel channel(server_sock.GetSocket(),rcb);
    channel.setEvent(POLLIN);

    EventLoop main_loop;
    main_loop.addNewChannel(&channel);
    main_loop.startLoop();

    time_t end_time=time(0);
    tm=localtime(&end_time);
    printf("server end time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);
    return 0;
}
