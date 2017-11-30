#include "tcp_socket.h"
#include <cstdint>
namespace net {
bool TcpSocket::CreateSocket(int port, int af, int type)
{
    fd_ = socket(af, type, 0);
    if (fd_ < 0)
        return false;
    int on = 1;
    if ((setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        return false;
    }
    sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(static_cast<uint16_t>(port));

    int flag = bind(fd_, (sockaddr*) &sa, sizeof(sa));
    return flag >= 0;
}

void TcpSocket::Listen(int backlog)
{
    listen(fd_, backlog);
}

ssize_t TcpSocket::Receive(void* buffer, size_t buf_len)
{
    return recv(fd_, static_cast<char*>(buffer), buf_len, 0);
}

ssize_t TcpSocket::Receive(net::SocketBuf& buf)
{
    ssize_t size = recv(fd_, buf.writeBegin(), buf.writeableBytes(), 0);
    buf.writeSkip(size);
    return size;
}

int TcpSocket::Accept(char* fromIP, UINT& fromPort)
{
    assert(fromIP != NULL);
    sockaddr_in from;
    memset(&from, 0, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    socklen_t len = sizeof(from);
    int clientSock = -1;
    if ((clientSock = accept(fd_, (sockaddr*) &from, &len)) < 0)
        return clientSock;
    strcpy(fromIP, inet_ntoa(from.sin_addr));
    fromPort = htons(from.sin_port);
    return clientSock;
}

ssize_t TcpSocket::Send(const void* message, size_t buf_len)
{
    return send(fd_, message, buf_len, 0);
}

bool TcpSocket::GetPeerName(char* peerIP, UINT& peerPort)
{
    sockaddr_in from;
    memset(&from, 0, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    socklen_t len = sizeof(from);
    if (getpeername(fd_, (sockaddr*) &from, &len) < 0) {
        return false;
    }
    else {
        strcpy(peerIP, inet_ntoa(from.sin_addr));
        peerPort = htons(from.sin_port);
    }
    return true;;
}

bool TcpSocket::Connect(const char* conIP, const UINT conPort)
{
    sockaddr_in conAddr;
    memset(&conAddr, 0, sizeof(struct sockaddr_in));
    conAddr.sin_family = AF_INET;
    conAddr.sin_addr.s_addr = inet_addr(conIP);
    conAddr.sin_port = htons(static_cast<uint16_t>(conPort));
    socklen_t len = sizeof(conAddr);
    int flag = connect(fd_, (sockaddr*) &conAddr, len);
    return flag >= 0;
}

TcpSocket::~TcpSocket()
{
}
int TcpSocket::getFd() const
{
    return fd_;
}
ssize_t TcpSocket::write_n(const void* msg, size_t buf_len)
{
    ssize_t send_size = 0;
    ssize_t a_send_size;
    while ((a_send_size = ::write(fd_,(char*)msg+send_size, buf_len-send_size)) > 0)
    {
        send_size += a_send_size;
        if (send_size == buf_len)
            break;
    }
    return send_size;
}
ssize_t TcpSocket::read_n(void* msg, size_t buf_len)
{
    ssize_t recv_size = 0;
    ssize_t a_recv_size;
    while ((a_recv_size = ::read(fd_,(char*)msg + recv_size, buf_len-recv_size)) > 0)
    {
        recv_size += a_recv_size;
        if (recv_size == buf_len )
            break;
    }
    return recv_size;
}
}//namespace net



