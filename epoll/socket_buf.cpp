//
// Created by yangning on 17-11-7.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include "socket_buf.h"
#include <assert.h>

namespace net{

bool net::SocketBuf::read(char* data, size_t bytes)
{
    assert(data != NULL && bytes >= 0);
    if(bytes > readableBytes())
        return false;
	memcpy(data, readBegin(), bytes);
    readIndex_ += bytes;
    return true;
}

bool net::SocketBuf::read(net::SocketBuf & buf, size_t bytes)
{
	assert(bytes >= 0);
	if (bytes > readableBytes())
		return false;
	read(buf.writeBegin(), bytes);
	buf.writeIndex_ += bytes;
	return true;
}

void net::SocketBuf::write(const char* data, size_t len)
{
	assert(data != NULL&&len > 0);
	if (len > writeableBytes())
	{
		grow(len);
	}
	memcpy(buffer_+writeIndex_, data,len);
	writeIndex_ += len;
}

bool net::SocketBuf::write(net::SocketBuf & buf, size_t bytes)
{ 
	assert(bytes> 0);
	if (bytes > writeableBytes())
		return false;
	write(buf.readBegin(), bytes);
	buf.readIndex_ += bytes;
	return true;
}

void SocketBuf::grow(size_t len)
{
	if (writeableBytes() + prependBytes() < len + kPrepend)
	{
		int size = capacity_*kGrowthFactor + len;
		char* buf = new char[size];
		assert(buf != NULL);
		memcpy(buf, buffer_, capacity_);
		delete[] buffer_;
		buffer_ = buf;
		capacity_ =size ;
	}
	else
	{
		size_t readable = readableBytes();
		memmove(begin() + kPrepend, readBegin(), readable);
		readIndex_ = kPrepend;
		writeIndex_ = readIndex_ + readable;
	}
}

}//namespace net