#include "event_loop.h"


bool EventLoop::isLoopInThisThread()
{
    return threadId_==getpid();
}


void EventLoop::startLoop()
{
    isLooping_=true;
    while(isLooping_)
    {
        if(!acticveChannels_.empty())
            acticveChannels_.clear();
        poller_->Poll(kTimeOut,acticveChannels_);
        handleEvent();
    }
}

void EventLoop::addNewChannel(Channel* channel)
{
    if(channel != NULL)
        poller_->addNewChannel(channel);
}


void EventLoop::handleEvent()
{
    if(acticveChannels_.empty())
        return;
    for(auto i : acticveChannels_)
    {
        i->handleEvent();
    }
}
