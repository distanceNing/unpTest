
#include "channel.h"


#include <poll.h>





void Channel::handleEvent()
{
    if(revents&POLLIN)
    {
        readCallBack();
    }
    if(revents&POLLOUT)
    {
        writeCallBack();
    }
    if(revents&POLLERR)
    {
        errorCallBack();
    }
}
