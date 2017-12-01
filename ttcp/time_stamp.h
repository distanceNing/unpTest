//
// Created by yangning on 17-10-23.
//

#ifndef BASE_NET_LIB_TIMESTAMP_H
#define BASE_NET_LIB_TIMESTAMP_H

#include <sys/time.h>
#include <cstdint>
#include <ctime>

namespace net {
class TimeStamp {

public:
    using PassingTime=time_t;

    TimeStamp()
            :time_((int64_t)time(0))
    {
    }

    static void printTimeNow();

    void printTime();

    PassingTime operator-(TimeStamp& rvalue);

    ~TimeStamp()
    {
    }

private:
    int64_t time_;
};
}//namespace net
#endif //!BASE_NET_LIB_TIMESTAMP_H
