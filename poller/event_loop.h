#include <vector>

#include "channel.h"
#include "poller.h"

const int kTimeOut=5*1000;



class EventLoop
{
public:
    EventLoop():isLooping_(false),threadId_(getpid()){}

    bool isLoopInThisThread();

    void startLoop();

    void quitLoop()
    {
        isLooping_=false;
    }

    void handleEvent();
protected:
    typedef std::vector<Channel*> ChannelList;
private:
    bool isLooping_;
    const pid_t threadId_;
    ChannelList acticveChannels_;
    Poller poller_;
};

