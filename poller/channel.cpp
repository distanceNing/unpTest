
#include "channel.h"


#include <poll.h>





void Channel::handleEvent()
{
    if(revents_&POLLIN)
    {
        readCallBack_(fd_);
    }
    if(revents_&POLLOUT)
    {
        writeCallBack_(fd_);
    }
    if(revents_&POLLERR)
    {
        errorCallBack_(fd_);
    }
}
