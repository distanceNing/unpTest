//
// Created by yangning on 17-11-7.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include "socket_buf.h"
#include <assert.h>
bool net::SocketBuf::readBytes(char* data, size_t bytes)
{
    assert(data != NULL && bytes >= 0);
    if(bytes > readableBytes())
        return false;
    std::copy(dataVec_.data()+readIndex_,dataVec_.data()+readIndex_+bytes,data);
    readIndex_ += bytes;
    return true;
}

bool net::SocketBuf::readBytes(net::SocketBuf & buf, size_t bytes)
{
	assert(bytes >= 0);
	if (bytes > readableBytes())
		return false;
	return buf.writeBytes(dataVec_.data() + readIndex_, bytes);;
}

bool net::SocketBuf::writeBytes(const char* data, size_t len)
{
	assert(data != NULL&&len > 0);
	if (len > writeableBytes()) 
		 return false;
	std::copy(data, data + len, &*dataVec_.begin() + writeIndex_);
	writeIndex_ += len;
	return true;
}

bool net::SocketBuf::writeBytes(net::SocketBuf & buf, size_t bytes)
{ 
	assert(bytes> 0);
	if (bytes > writeableBytes())
		return false;
	writeBytes(buf.dataVec_.data() + buf.readIndex_, bytes);
	buf.writeIndex_ -= bytes;
	return true;
}
