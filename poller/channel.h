
#include <boost/utility.hpp>

typedef void (*EventCallBack)(int fd);

class Channel:public boost::noncopyable
{

public:
    Channel(const int fd,
            EventCallBack wcb,
            EventCallBack rcb,
            EventCallBack ecb
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
