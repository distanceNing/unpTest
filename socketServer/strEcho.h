#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/select.h>

#define BUF_SIZE 1024

typedef void Sigfunc(int);

void sigChild(int signo);



void strEcho(int sock);
