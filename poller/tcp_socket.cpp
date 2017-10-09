#include "tcp_socket.h"

#ifndef _WIN32
unsigned int GetLastError()
{
	return errno;
}
#endif // !_WIN32

Csocket::Csocket()
{
	sock = -1;
#ifdef _WIN32
	WSADATA iData = { 0 };
	int iResult = WSAStartup(0x0202, &iData);
#endif // !_WIN32

}


Csocket::~Csocket()
{
#ifdef _WIN32
	closesocket(sock);

#else
	close(sock);
#endif 
}


bool Csocket::CreatSocket(int af, int type, int prot)
{
	sock = socket(af, type, 0);   
	if (sock == SOCKET_ERROR)
		return false;        
	sockaddr_in sa = { af,htons(prot) };
	int flag = bind(sock, (sockaddr*)&sa, sizeof(sa));
	if (flag<0)
	{	
		return false;
	}
	return true;
}


int Csocket::Receive(void *buffer,int buflen)
{
	return recv(sock, (LPSTR)buffer, buflen, 0);
}


bool Csocket::Sendto(const void *message, const char* toIP, const UINT toPort)
{
	sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_port =htons(toPort);
#ifdef _WIN32
	to.sin_addr.S_un.S_addr = inet_addr(toIP);

#else
	to.sin_addr.s_addr= inet_addr(toIP);
#endif 

	int flag=sendto(sock, (char *)message, sizeof(message), 0, (sockaddr*)&to, sizeof(to));
	if (flag > 0)
	{
		return true;
	}
	return false;
}


int Csocket::ReceiveFrom(void *buffer, int buflen, char* fromIP,UINT &fromPort)
{
	sockaddr_in from = { AF_INET };
	socklen_t len = sizeof(from);
	int size = recvfrom(sock, (char*)buffer, buflen, 0, (sockaddr*)&from, &len);
	if (size < 0)
		return 0;
	if (size > 0)
	{
		strcpy(fromIP, inet_ntoa(from.sin_addr));
		fromPort = ntohs(from.sin_port);
	}
	return size;
}


bool Csocket::Listen(int backlog)
{
	listen(sock, backlog);
	return true;
}

bool Csocket::Accept(Csocket & socka, char * fromIP, UINT & fromPort)
{
	sockaddr_in from = { AF_INET };
	socklen_t len = sizeof(from);
	socka.sock = accept(sock,(sockaddr*)&from, &len);
	strcpy(fromIP,inet_ntoa(from.sin_addr));
	fromPort = htons(from.sin_port);
	return true;
}


size_t Csocket::Send(void* message, int buflen)
{
	return send(sock, (LPCSTR)message, buflen, 0) ;
}


bool Csocket::GetPeerName(char* peerIP, UINT &peerPort)
{
	sockaddr_in from = { AF_INET };
	socklen_t len = sizeof(from);
	if (getpeername(sock, (sockaddr*)&from, &len)<0)
	{
		return false;
	}
	else
	{
		strcpy(peerIP, inet_ntoa(from.sin_addr));
		peerPort=htons(from.sin_port);
	}
	return true;;
}

bool Csocket::Connect(char* conIP, UINT conPort)
{
	sockaddr_in conAddr = { AF_INET };
#ifdef _WIN32
	conAddr.sin_addr.S_un.S_addr = inet_addr(conIP);

#else
	conAddr.sin_addr.s_addr = inet_addr(conIP);
#endif 
	conAddr.sin_port = htons(conPort);
	socklen_t len = sizeof(conAddr);
	int flag = connect(sock, (sockaddr*)&conAddr, len);
	if (flag<0)
		return false;
	return true;
}


bool Csocket::CloseSocket()
{
#ifdef _WIN32
	closesocket(sock);

#else
	close(sock);
#endif 
	return true;
}
