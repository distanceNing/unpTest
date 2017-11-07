//
// Created by yangning on 17-11-7.
//
// Descriprion :user socket buffer 
//
// Copyright (c) yangning All rights reserved.
//

#ifndef EPOLL_SOCKETBUF_H
#define EPOLL_SOCKETBUF_H

#include <vector>
#include <algorithm>

namespace net {

class SocketBuf
{
public:
    SocketBuf(size_t init_size = 1024, size_t prepend = 8)
            :readIndex_(prepend), writeIndex_(prepend), dataVec_(prepend + init_size)
    {
    }

    bool writeBytes(const char* data, size_t len);

	bool writeBytes(net::SocketBuf& buf, size_t bytes);

    bool readBytes(char* data, size_t bytes);

	bool readBytes(net::SocketBuf& buf, size_t bytes);

    ~SocketBuf()
    {
    }
private:
    std::vector<char> dataVec_;
    size_t readIndex_;
    size_t writeIndex_;

    size_t readableBytes() const
    {
        return writeIndex_ - readIndex_;
    }

    size_t writeableBytes() const
    {
        return dataVec_.size() - writeIndex_;
    }
};

} //namespace net

#endif //EPOLL_SOCKETBUF_H
