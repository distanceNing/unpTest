#include "strEcho.h"


void sigChild(int signo)
{
    pid_t pid;
    int stat;
    pid=wait(&stat);
    printf("child %d terminated\n",pid);
    return;
}


void strEcho(int sock)
{
    ssize_t read_size=-1;
    char buffer[BUF_SIZE];

again:
    while((read_size=read(sock,buffer,BUF_SIZE))>0)
    {
        write(sock,buffer,read_size);
    }
    if(read_size < 0 && errno == EINTR)
       goto again;
   else
      if(read_size < 0)
         perror("read errno"); 
}
