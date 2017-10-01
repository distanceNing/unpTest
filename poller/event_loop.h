#include <vector>

#include "poller.h"

const int kTimeOut=5*1000;

class Channel;
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
};

