#include "print_time.h"
#include "event_loop.h"
void wcb(int fd)
{
    char buffer[]="hello world";
    size_t size=write(fd,buffer,strlen(buffer));
    printf("writeable write size is : %lu\n",size);
}

void rcb(int fd)
{
    char buffer[256]={'\0'};
    size_t size=read(fd,buffer,256);
    printf("readable read  size is : %lu\n",size);
}
void ecb(int fd)
{
    printf("fd %d error\n",fd);
}

int main()
{
    time_t start_time=time(0);
    struct tm* tm=localtime(&start_time);
    printf("server start time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);

    EventLoop main_loop;
    main_loop.startLoop();
    if(!main_loop.isLoopInThisThread())
    {
        printf("Not loop this thread \n");
        std::abort();
    }
    /*
       int listenfd;
       int flag;
       struct pollfd* clientfd;
       struct sockaddr_in serveraddr;

       listenfd=socket(AF_INET,SOCK_STREAM,0);

       memset(&serveraddr,0,sizeof(struct sockaddr_in));
       serveraddr.sin_family=AF_INET;
       serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
       serveraddr.sin_port=htons(SERVER_PORT);

       bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
       listen(listenfd,5);


       clientfd=initPollArray(OPEN_MAX);
       if(clientfd==NULL)
       {
       printf("initPollArray failed !\n");
       return -1;
       }
       printf("initPollArray success ! \n");
       flag=pollHandleConnect(clientfd,listenfd,OPEN_MAX);
       free(clientfd);
       */
    time_t end_time=time(0);
    tm=localtime(&end_time);
    printf("server end time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);
    return 0;
}
