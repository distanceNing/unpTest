
#include <map>

#include <boost/utility.hpp>


typedef void (*EventCallBack)();

class Channel:public boost::noncopyable
{

public:
    Channel(){}

    
    void setWriteCallBack(EventCallBack call_back)
    {
        writeCallBack=call_back;
    }

    void setReadCallBack(EventCallBack call_back)
    {
        readCallBack=call_back;
    }

    void setErrorCallBack(EventCallBack call_back)
    {
        errorCallBack=call_back;
    }

    void handleEvent();


    ~Channel(){}
    
private:

    int fd;
    int events;
    int revents;
    EventCallBack writeCallBack;
    EventCallBack readCallBack;
    EventCallBack errorCallBack;

};
