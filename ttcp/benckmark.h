//
// Created by yangning on 17-12-30.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#ifndef TTCP_BENCKMARK_H
#define TTCP_BENCKMARK_H
#include "socket/epoller.h"
#include "socket/tcp_socket.h"

#include <memory>
#include <cstdio>

namespace  net{
class TimerFd;
}

struct Option {
  const char* server_ip_;
  uint16_t server_port_;
  bool is_keep_alive_;
  size_t thread_num_;
  size_t concurrent_num_;
  size_t connect_timeout_ms_;
  uint32_t interval_time_s_;
  Option()
  {
      server_ip_ = "127.0.0.1";
      connect_timeout_ms_ = 3000;
      server_port_ = 9000;
      concurrent_num_ = 1024;
      is_keep_alive_ = true;
      thread_num_ = 1;
      interval_time_s_ = 0;
  }

  static void printDefautOption()
  {

      printf("Defaut Option : \n");
      printf("\thost               = 127.0.0.1\n"
              "\ttimeout_ms         = 3000\n"
              "\tserver_port        = 9000\n"
              "\tconcurrent_num     = 1024\n"
              "\tis_keep_alive      = true\n"
              "\tthread_num         = 1\n");
      printf("Usage: %s \n"
              "\t  [-h host] \n"
              "\t  [-p source_port] \n"
              "\t  [-c concurrent_num] \n"
              "\t  [-t thread_num] \n"
              "\t  [-a is_keep_alive] \n"
              "\t  [-w timeout] \n", "benchmark");
  }
};

class BenchMark {
public:
    BenchMark()
            :isLoop_(false)
    {
    }
    //当套接字可读时,用户制定的回调函数
    void setReadCallBack(const Epoll::ReadCallBack& cb)
    {
        readCallBack_ = cb;
    }
    //可写时
    void setWriteCallBack(const Epoll::WriteCallBack& cb)
    {
        writeCallBack_ = cb;
    }

    //调用run使得测试运行起来
    void run();

    void setConcurrentNum(size_t num)
    {
        option_.concurrent_num_ = num;
    }
    void setIntervalTimes(uint32_t time)
    {
        option_.interval_time_s_=time;
    }

    void setUserOption(int argc, char** argv)
    {

    }
private:
    void benchmark();
    //std::unique_ptr<net::TimerFd> timer_;
    Option option_;
    bool isLoop_;
    Epoll::ReadCallBack readCallBack_;
    Epoll::WriteCallBack writeCallBack_;
};

#endif //TTCP_BENCKMARK_H
