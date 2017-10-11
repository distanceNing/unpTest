#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
const int kTimeOut = 3;
#include "timer.h"

int main()
{
    Timer timer;
    timer.createTimer();
    timer.setTIme(4,kTimeOut);

    uint64_t error = 0;
    ssize_t read_size=read(timer.getTimerFd(),&error,sizeof(uint64_t));
    if(read_size != sizeof(uint64_t))
    {
        perror("read error");
    }

    return 0;
}
