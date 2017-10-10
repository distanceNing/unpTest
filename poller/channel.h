#ifndef _CHINELL_H_
#define _CHINELL_H_
#include <boost/utility.hpp>

typedef void (*EventCallBack)(int fd);

class Channel: boost::noncopyable
{

public:
    Channel(const int fd,
            EventCallBack rcb=NULL,
            EventCallBack wcb=NULL,
            EventCallBack ecb=NULL
            ):fd_(fd),writeCallBack_(wcb),readCallBack_(rcb),errorCallBack_(ecb){}

    
    void setWriteCallBack(EventCallBack call_back)
    {
        writeCallBack_=call_back;
    }

    void setReadCallBack(EventCallBack call_back)
    {
        readCallBack_=call_back;
    }

    void setErrorCallBack(EventCallBack call_back)
    {
        errorCallBack_=call_back;
    }
    
    void setEvent(int events)
    {
        events_ = events;
    }

    void serRetEvents(int revents)
    {
        revents_ = revents;
    }
 
    int getEvents() const
    {
        return events_;
    }
    void handleEvent();

    int getFd() const 
    {
        return fd_;
    }

    ~Channel(){}
    
private:

    const int fd_;
    int events_;
    int revents_;
    EventCallBack writeCallBack_;
    EventCallBack readCallBack_;
    EventCallBack errorCallBack_;
};
#endif//!_CHINELL_H_
