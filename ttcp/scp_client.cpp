#include <iostream>
#include "socket/tcp_socket.h"
#include "common.h"


int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "argc < 4 !\n ";
        return -1;
    }
    const char* local_file_name = argv[2];

    net::TcpSocket sock;
    sock.CreateSocket();
    char* fromIP = argv[1];
    char* file_name = argv[3];
    UINT fromPort = kPort;
    if (!sock.Connect(fromIP, fromPort)) {
        printErrorMsg("connect");
    }
    sock.Send(file_name,strlen(file_name));
    int ack;
    sock.Receive(&ack,sizeof ack);
    ::sleep(5);
    std::cout<<"start send file ---- \n";
    ssize_t size = send_file(local_file_name, sock.getFd());
    if (size < 0)
        printErrorMsg("sendfile");
    std::cout << "send size: " << size << std::endl;
    std::cout << "copy successful!\n";

    sock.shutDownWrite();
    //safe close connection
    char buffer[MAX_BUF_SIZE];
    while(sock.Receive(buffer,MAX_BUF_SIZE) > 0)
    {
        //DO NOTHING
    }

    sock.closeFd();
    return 0;

}
