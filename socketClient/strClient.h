#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 1024


int max(int,int);

void strClient(FILE* fp,int sock);
