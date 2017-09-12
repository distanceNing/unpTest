#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>


#include <vector>

#define OPEN_MAX 10

#define BUF_SIZE 1024

#define MAYBE_TIME_OUT -1

typedef int ERROR_TYPE; 

class Poller
{
public:
    Poller(int openMax):pollList_(openMax){}

private:

    std::vector<struct pollfd> pollList_;
};

struct pollfd* initPollArray(int openMax);

ERROR_TYPE pollHandleConnect(struct pollfd* clientfd,int listenfd,int openMax);


