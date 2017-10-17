#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")
typedef int socklen_t;

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef int SOCKET;
typedef unsigned int UINT;
typedef const char* LPCSTR;
typedef char* LPSTR;
const int SOCKET_ERROR = -1;
unsigned int GetLastError();
#endif

class TcpSocket {
    SOCKET sock;
public:

    TcpSocket();s
    ~TcpSocket();
    bool CreateSocket(int af = AF_INET, int type = SOCK_STREAM, int prot = 0);
    int Receive(void* buffer, int buflen);
    bool Sendto(const void* message, const char* toIP, const UINT toPort);
    int ReceiveFrom(void* buffer, int buflen, char* fromIP, UINT& fromPort);
    bool Listen(int backlog = 5);
    bool Accept(TcpSocket& socka, char* fromIP, UINT& fromPort);
    size_t Send(void* message, int buflen);
    bool GetPeerName(char* peerIP, UINT& peerPort);
    bool Connect(char* conIP, UINT conPort);
    bool CloseSocket();
    int GetSocket() const
    {
        return sock;
    }
};
#endif //_TCP_SOCKET_H_
