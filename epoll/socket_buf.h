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
#include <string>
#include <algorithm>
#include <assert.h>
namespace net {
	
class SocketBuf
{
public:
	static const size_t kInitSize = 1024;
	static const size_t kPrepend = 8;
    SocketBuf(size_t init_size = kInitSize, size_t prepend = kPrepend)
            :readIndex_(prepend), writeIndex_(prepend), capacity_(prepend+init_size)
    {
		buffer_ = new char[capacity_];
    }
	//write
	void append(const std::string str)
	{
		write(str.c_str(), str.size());
	}

    void write(const char* txt, size_t len);

	bool write(net::SocketBuf& buf, size_t bytes);

	//read
	void skip(size_t len)
	{
		readIndex_ += len;
	}

    bool read(char* txt, size_t bytes);

	bool read(net::SocketBuf& buf, size_t bytes);

	std::string read(size_t len)
	{
		assert(readableBytes() > len);
		return std::string(readBegin(), len);
	}

	int32_t readInt32()
	{
		int32_t ret = 0;
		memcpy(&ret, readBegin(), sizeof int32_t);
		skip(sizeof int32_t);
		return ret;
	}

    ~SocketBuf()
    {
		delete[] buffer_;
		buffer_ = nullptr;
		capacity_ = 0;
    }
private:
	const int kGrowthFactor = 1.5;

	void grow(size_t len);
	
	const char* begin() const
	{
		return buffer_;
	}

	char* begin() 
	{
		return buffer_;
	}

	char* readBegin() 
	{
		return begin() + readIndex_;
	}

	const char* readBegin() const
	{
		return begin() + readIndex_;
	}

	char* writeBegin()
	{
		return begin() + writeIndex_;
	}

	const char* writeBegin() const
	{
		return begin() + writeIndex_;
	}

	size_t readableBytes() const
	{
		return writeIndex_ - readIndex_;
	}

	size_t writeableBytes() const
	{
		return capacity_ - writeIndex_;
	}

	size_t prependBytes()const
	{
		assert(readIndex_ >= kPrepend);
		return readIndex_ - kPrepend;
	}

	char*  buffer_;
	size_t capacity_;
    size_t readIndex_;
    size_t writeIndex_;

};

} //namespace net

#endif //EPOLL_SOCKETBUF_H
