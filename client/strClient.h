#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#define BUF_SIZE 1024


void strClient(FILE* fp,int sock);
