#include "benckmark.h"
#include "thread/thread_pool.h"
#include "socket/tcp_socket.h"
#include "timerfd/timer_fd.h"
void BenchMark::run()
{
   isLoop_= true;
    if ( option_.thread_num_ == 1 ) {
        benchmark();
    }
    else {
        ThreadPool threadPool(std::bind(&BenchMark::benchmark,this), option_.thread_num_);
        threadPool.run();
        threadPool.join();
    }
}
void BenchMark::benchmark()
{
    Epoll epoll(option_.interval_time_s_);
    //if ( option_.interval_time_s_ != 0 )
    //{
    //    timer_ = new net::TimerFd();
    //    timer_->setTime(option_.interval_time_s_,option_.interval_time_s_);
    //}


    //向主机发起连接
    for (int i = 0; i < option_.concurrent_num_; ++i) {
        int conn_fd;
        if ((conn_fd = net::TcpSocket::create_and_bind()) < 0 )
            printErrorMsg("creat socket");
        if ( net::TcpSocket::noblockingConnect(conn_fd, option_.server_ip_, option_.server_port_,
                option_.connect_timeout_ms_) < 0 ) {
            printErrorMsg("connect");
        }
        if ( !setFdNonBlocking(conn_fd))
            printErrorMsg("setFdNonBlocking");
        printf("connect time is %d \n", i);
        net::TcpSocket::setTcpNoDelay(conn_fd);
        epoll.addNewFd(conn_fd);
        usleep(1000);
    }

    epoll.setReadcb(readCallBack_);
    epoll.setWritecb(writeCallBack_);
    while (isLoop_)
        epoll.epollWait();
}

