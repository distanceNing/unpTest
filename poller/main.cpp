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

    //建立监听socket
    Csocket server_sock;
    server_sock.CreatSocket(kPort);
    server_sock.Listen();

    EventLoop main_loop;
    main_loop.startLoop();

    time_t end_time=time(0);
    tm=localtime(&end_time);
    printf("server end time :%d : %d : %d \n",tm->tm_hour,tm->tm_min,tm->tm_sec);
    return 0;
}
