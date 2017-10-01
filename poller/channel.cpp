
#include "channel.h"


#include <poll.h>





void Channel::handleEvent()
{
    if(revents_&POLLIN)
    {
        readCallBack_();
    }
    if(revents_&POLLOUT)
    {
        writeCallBack_();
    }
    if(revents_&POLLERR)
    {
        errorCallBack_();
    }
}
