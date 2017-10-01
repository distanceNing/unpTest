#include "event_loop.h"


bool EventLoop::isLoopInThisThread()
{
    return threadId_==getpid();
}


void EventLoop::startLoop()
{
    isLooping_=true;
    //    while(isLooping_)
    //    {
    ::poll(NULL,0,kTimeOut);
    //   }

}

void EventLoop::handleEvent()
{
    for(auto i : acticveChannels_)
    {
        i->handleEvent();
    }
}
