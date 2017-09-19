#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <poll.h>


#include <vector>
#include <map>

#define OPEN_MAX 10

#define BUF_SIZE 1024

#define MAYBE_TIME_OUT -1

typedef int ERROR_TYPE; 

class Poller
{
public:
    Poller(int openMax):pollfdList_(openMax)
    {
     for(unsigned i=0;i<pollfdList_.size();i++)
        {
            pollfdList_[i].fd=-1;
        }
    }

    int Poll(int time_out);    

    ~Poller(){}

private:

    std::vector<struct pollfd> pollfdList_;
};

struct pollfd* initPollArray(int openMax);

ERROR_TYPE pollHandleConnect(struct pollfd* clientfd,int listenfd,int openMax);


