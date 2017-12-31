#include "tcp_socket.h"
#include <cstdint>
namespace net {
bool SocketFd::CreateSocket(int port, int af, int type)
{
	fd_ = socket(af, type, 0);
	if (fd_ == SOCKET_ERROR)
		return false;

	int on=1;
    return setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) >= 0;
    //sockaddr_in sa;
	//memset(&sa, 0, sizeof(struct sockaddr_in));
	//sa.sin_family = AF_INET;
	//sa.sin_port = htons(static_cast<uint16_t>(port));

	//int flag = bind(fd_, (sockaddr*)&sa, sizeof(sa));
	//return flag >= 0;
}

void SocketFd::Listen(int backlog)
{
	listen(fd_, backlog);
}

ssize_t SocketFd::Receive(void* buffer, size_t buf_len)
{
	return recv(fd_, static_cast<char*>(buffer), buf_len, 0);
}

ssize_t SocketFd::Receive(net::SocketBuf& buf)
{
	ssize_t size= recv(fd_, buf.writeBegin(), buf.writeableBytes(), 0);
	buf.writeSkip(size);
	return size;
}

int SocketFd::Accept(char* fromIP, UINT& fromPort)
{
	sockaddr_in from;
	memset(&from, 0, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	socklen_t len = sizeof(from);
	int clientSock = -1;
	if ((clientSock = accept(fd_, (sockaddr*)&from, &len)) < 0)
		return clientSock;
	strcpy(fromIP, inet_ntoa(from.sin_addr));
	fromPort = htons(from.sin_port);
	return clientSock;
}

ssize_t SocketFd::Send(const char* message, size_t buf_len)
{
	return send(fd_, message, buf_len, 0);
}

bool SocketFd::GetPeerName(char* peerIP, UINT& peerPort)
{
	sockaddr_in from;
	memset(&from, 0, sizeof(struct sockaddr_in));
	from.sin_family = AF_INET;
	socklen_t len = sizeof(from);
	if (getpeername(fd_, (sockaddr*)&from, &len) < 0)
	{
		return false;
	}
	else {
		strcpy(peerIP, inet_ntoa(from.sin_addr));
		peerPort = htons(from.sin_port);
	}
	return true;;
}

bool SocketFd::Connect(const char* conIP, const UINT conPort)
{
	sockaddr_in conAddr ;
	memset(&conAddr, 0, sizeof(struct sockaddr_in));
	conAddr.sin_family = AF_INET;
	conAddr.sin_addr.s_addr = inet_addr(conIP);
	conAddr.sin_port = htons(static_cast<uint16_t>(conPort));
	socklen_t len = sizeof(conAddr);
	int flag = connect(fd_, (sockaddr*)&conAddr, len);
	return flag >= 0;
}


SocketFd::~SocketFd()
{
#ifdef _WIN32
	closesocket(fd_);
#else
	close(fd_);
#endif // _WIN32


}
int SocketFd::getFd() const
{
	return fd_;
}
}//namespace net



