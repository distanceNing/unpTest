//
// Created by yangning on 17-10-18.
//

#ifndef BASE_NET_LIB_COMMON_H
#define BASE_NET_LIB_COMMON_H
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#define MAX_BUF_SIZE 1024

typedef struct {
  int32_t length;
  int32_t number;
} SessionInfo;

typedef struct {
  int32_t length;
  char content[0];
} PayloadInfo;

const int kPort = 9000;
inline void printErrorMsg(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

inline bool setFdNonBlocking(int fd)
{
    int flag = fcntl(fd, F_SETFL, O_NONBLOCK);
    if (flag == -1) {
        printErrorMsg("setFdNonBlocking");
    }
    return true;
}

struct IpAddress {
  std::string ip;
  unsigned int port;

  IpAddress(const char* ip, unsigned int port)
          :ip(ip), port(port)
  {
  }
};

#endif //BASE_NET_LIB_COMMON_H
