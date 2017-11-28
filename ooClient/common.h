//
// Created by yangning on 17-10-18.
//

#ifndef BASE_NET_LIB_COMMON_H
#define BASE_NET_LIB_COMMON_H

#define MAX_BUF_SIZE 1024
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>

inline void printErrorMsg(const char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
#ifndef _WIN32

bool setFdNonBlocking(int fd);

#endif // !_WIN32


#endif //BASE_NET_LIB_COMMON_H
