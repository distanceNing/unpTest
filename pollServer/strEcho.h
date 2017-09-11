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
#include <poll.h>
#include <stdlib.h>
#include <stdbool.h>

#define OPEN_MAX 10

#define BUF_SIZE 1024

#define MAYBE_TIME_OUT 5000

typedef int ERROR_TYPE; 


struct pollfd* initPollArray(int openMax);

ERROR_TYPE pollHandleConnect(struct pollfd* clientfd,int listenfd,int openMax);


