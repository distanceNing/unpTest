//
// Created by yangning on 17-12-2.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include <csignal>
#include "benckmark.h"

#define   HEAD_SIZE   7


static int
process_parameters (int argc, char **argv)
{
    int c;
    while ((c = getopt(argc, argv, "h:p:t:c:a")) != -1) {
        switch (c) {
        case 'a':gOption.is_keep_alive_ = true;
            break;
        case 'h':gOption.server_ip_ = optarg;
            break;
        case 't':gOption.thread_num_ = (size_t) atoi(optarg);
            break;
        case 'p':gOption.server_port_ = (uint16_t) atoi(optarg);
            break;
        case 'c':gOption.concurrent_num_ = (uint16_t) atoi(optarg);
            break;
        default:Option::printDefautOption();
            return 0;
        }
    }

    return 0;
}



int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        Option::printDefautOption();
    }
    else
        process_parameters(argc,argv);

    BenchMark benchMark;

    //构造一个心跳包
    char buffer[MAX_BUF_SIZE] = {'\0'};
    const char* cmdType = "HBT";
    const char* text = "HBT";
    const int CMD_SIZE = 3;
    size_t text_len = 3;
    int net_len = ntohl((uint32_t) text_len);
    memcpy(buffer, cmdType, CMD_SIZE);
    memcpy(buffer + CMD_SIZE, &net_len, sizeof(int));
    memcpy(buffer + HEAD_SIZE, text, text_len);
    size_t pktSize = (size_t) (HEAD_SIZE + text_len);

    benchMark.setReadCallBack([buffer,pktSize](net::SocketBuf* buf, int fd) {
      char recv_buf[MAX_BUF_SIZE] = {'\0'};
      size_t readable = buf->readableBytes();
      buf->read(recv_buf, readable);
      printf("%s\n", recv_buf);
      ::send(fd, buffer, pktSize, MSG_NOSIGNAL);
      if(gOption.is_keep_alive_)
      {}
    });


    benchMark.setWriteCallBack([buffer, pktSize](int fd) {
      ::send(fd, buffer, pktSize, MSG_NOSIGNAL);
    });

    benchMark.run();
    return 0;
}

