#ifndef _BASE_NET_LIB_SOCKET_FD_H_
#define _BASE_NET_LIB_SOCKET_FD_H_

#ifdef _WIN32

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")
typedef int socklen_t;
typedef size_t ssize_t;

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
typedef unsigned int SOCKET;
typedef unsigned int UINT;
typedef const char* LPCSTR;
typedef char* LPSTR;

#endif 

#include "socket_buf.h"
namespace net {


class SocketFd {
protected:
	typedef unsigned int UINT;
#ifndef _WIN32
	static const int SOCKET_ERROR = -1;
#endif // !_WIN32

public:
	SocketFd() {
#ifdef _WIN32
		WSADATA iData = { 0 };
		int iResult = WSAStartup(0x0202, &iData);
#endif // !_WIN32
	}

	explicit SocketFd(const int fd):fd_(fd) {
	}

	virtual ~SocketFd();

	bool CreateSocket(int port = 0, int af = AF_INET, int type = SOCK_STREAM);

	ssize_t Receive(void* buffer, size_t bufLen);

	ssize_t Receive(net::SocketBuf& buf);

	void Listen(int backlog = 5);

	int Accept(char* fromIP, UINT& fromPort);

	ssize_t Send(const char* message, size_t bufLen);

	bool GetPeerName(char* peerIP, UINT& peerPort);

	bool Connect(const char* conIP, const  UINT conPort);

	int getFd() const;

	void closeFd()
	{
#ifdef _WIN32
		closesocket(fd_);
#else
		close(fd_);
#endif // !_WIN32

	}
private:
	int fd_;

};
}//namespace net
#endif //_BASE_NET_LIB_SOCKET_FD_H_
