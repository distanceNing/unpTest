#include <iostream>
#include "Csocket.h"

#define SERVER_PORT 9000

int main()
{
    Csocket sock;
    sock.CreatSocket();
    char connIP[]="127.0.0.1";
    if(!sock.Connect(connIP,SERVER_PORT))
    {
        std::cout<<"Create Socket Error"<<strerror(errno)<<std::endl;
        return -1;
    }
    char buffer[256];
    std::cin>>buffer;
    sock.Send(buffer,strlen(buffer));
    memset(buffer,0,256);
    sock.Receive(buffer,256);
    std::cout<<buffer<<std::endl;
    sock.CloseSocket();    

}

