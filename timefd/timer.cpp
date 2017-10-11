#include "timer.h"
#include <stdlib.h>
#include <stdio.h>


inline void printErrMsg(const char * msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


bool Timer::createTimer()
{
    time_fd_ = timerfd_create(CLOCK_REALTIME,0);
    if(time_fd_ < 0)
    {
        printErrMsg("create timer fd");
        return false;
    }
    return true;
}

bool Timer::setTIme(const int after_time,const int timeout)
{
    struct itimerspec new_value;
    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
    {
        printErrMsg("clock gettime");
    }

    new_value.it_value.tv_sec = now.tv_sec + after_time;
    new_value.it_value.tv_nsec = now.tv_nsec;
   
    new_value.it_interval.tv_sec = timeout;
    new_value.it_interval.tv_nsec = 0;
    if (timerfd_settime(time_fd_, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
    {
        printErrMsg("timer fd settime");
        return false;
    }
    return true;
}


void printTime()
{
    static int call_count = 0;
    static struct timespec start;
    struct timespec now;
    if(call_count == 0)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        {
            printErrMsg("clock gettime");
        }
    }

    if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
    {
        printErrMsg("clock gettime");
    }
    ++call_count;

    printf("time : %ld  %ld\n",
            now.tv_sec-start.tv_sec,
            (now.tv_nsec-start.tv_nsec)/1000000);
}
