#include <vector>
#include <memory>

#include <boost/scoped_ptr.hpp>

#include "channel.h"
#include "poller.h"

const int kTimeOut=10*1000;



class EventLoop
{
public:
    EventLoop():isLooping_(false),threadId_(getpid()),poller_(new Poller){}

    ~EventLoop(){}

    bool isLoopInThisThread();

    void startLoop();

    void quitLoop()
    {
        isLooping_=false;
    }

    void handleEvent();

    void addNewChannel(Channel* channel);
protected:
    typedef std::vector<Channel*> ChannelList;
private:
    bool isLooping_;
    const pid_t threadId_;
    ChannelList acticveChannels_;
    boost::scoped_ptr<Poller> poller_;
};

