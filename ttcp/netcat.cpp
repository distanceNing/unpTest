#include "socket/epoller.h"
#include "socket/tcp_socket.h"
#include "common.h"
static int gUserCount = 0;

void stdincb(int fd);
void clientRcb(int fd, Epoll& epoll);

void listenRcb(int listenFd, Epoll* epoll);

static bool isgLoop = true;
int main(int argc, char* argv[])
{
    bool isClient = false;
    if (argc < 2) {
        std::cerr << "argc < 2   !\n ";
        return -1;
    }

    for (int i = 2; i < argc; ++i) {
        switch (*argv[i]) {
        case 'c':isClient = true;
            break;
        case 'l':isClient = false;
            break;
        default:break;
        }
    }

    const char* server_ip = "127.0.0.1"; //argv[1]
    net::TcpSocket sock;
    Epoll epoll;
    if ( isClient ) {
        if ( !sock.CreateSocket())
            printErrorMsg("creat socket");
        if ( !sock.Connect(server_ip, kPort))
            printErrorMsg("connect");
        Epoll::EventCallBack fun = std::bind(clientRcb, sock.getFd(), epoll);
        epoll.addNewFd(sock.getFd(), fun);
        //将stdin全部写入socket中
        Epoll::EventCallBack cb = std::bind(stdincb, sock.getFd());
        epoll.addNewFd(STDIN_FILENO, cb);
    }
    else {
        if ( !sock.CreateSocket(kPort))
            printErrorMsg("creat socket");
        sock.Listen();
        printf("wait for connection --- \n");
        Epoll::EventCallBack fun = std::bind(listenRcb, sock.getFd(), &epoll);
        epoll.addNewFd(sock.getFd(), fun);
    }
    while (isgLoop)
        epoll.epollWait();
    return 0;
}

void stdincb(int fd)
{
    char buffer[MAX_BUF_SIZE] = {'\0'};
    ssize_t read_size = ::read(STDIN_FILENO, buffer, MAX_BUF_SIZE);
    if ( read_size > 0 ) {
        ::write(fd, buffer, (size_t)read_size);
    }
    else if ( read_size < 0 )
        printErrorMsg("stdin");
};

void clientRcb(int fd, Epoll& epoll)
{
    std::cout << "client fd " << fd << " readable\n";
    //将socket上的东西写入标准输出
    char recv_buf[MAX_BUF_SIZE] = {'\0'};
    ssize_t read_size = read(fd, recv_buf, MAX_BUF_SIZE);
    if ( read_size == 0 ) {
        epoll.removeFd(fd);
        isgLoop = false;
        close(fd);
        std::cout << "client fd " << fd << " close --\n";
    }
    else if ( read_size > 0 ) {
        printf("%s", recv_buf);
    }
    else {
        printErrorMsg("read");
    }
}

void listenRcb(int listenFd, Epoll* epoll)
{
    char connIP[32] = {'\0'};
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd;
    if ((connfd = accept(listenFd, (struct sockaddr*) &cliaddr, &clilen)) < 0 ) {
        printErrorMsg("accept");
    }

    memset(connIP, '\0', 32);
    strcpy(connIP, inet_ntoa(cliaddr.sin_addr));
    printf("connect IP: %s ------ Port: %d\n", connIP, ntohs(cliaddr.sin_port));
    ++gUserCount;
    printf("accept -- UserCount : %d   \n", gUserCount);
    //将新连接的文件描述符加入到clienArray
    Epoll::EventCallBack fun = std::bind(clientRcb, connfd, *epoll);
    epoll->addNewFd(connfd, fun);
    Epoll::EventCallBack cb = std::bind(stdincb, connfd);
    epoll->addNewFd(STDIN_FILENO, cb);
}