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

#include "channel.h"

#define OPEN_MAX 10

#define BUF_SIZE 1024

#define MAYBE_TIME_OUT -1

typedef int ERROR_TYPE; 

class Poller
{
public:
    Poller()
    {
    }

    int Poll(int time_out);    

    void addNewChannel(Channel * channel);

    void removeChannel(const int fd_);

    ~Poller(){}

protected:
    typedef std::map<int,Channel*> ChannelMap;
    typedef std::vector<Channel*> ChannelList;
private:
    void FillActiveChannel(int num_ready,ChannelList &activeChannel);

    ChannelMap channelMap_;
    std::vector<struct pollfd> pollfdList_;
};

struct pollfd* initPollArray(int openMax);

ERROR_TYPE pollHandleConnect(struct pollfd* clientfd,int listenfd,int openMax);


