#include "is_the_server.h"
#include "Csocket.h"
#include <unistd.h>
#include <fcntl.h> 
#include <stdio.h>
int IsServerIP(const char* conIP,const int conPort,const int timeout)
{
	int ret = 0;


    Csocket sockClient;
    sockClient.CreatSocket();
    
	sockaddr_in conAddr = { AF_INET };
	conAddr.sin_addr.s_addr = inet_addr(conIP);
	conAddr.sin_port = htons(conPort);
    fd_set set;
    timeval tm;
    int flag = fcntl(sockClient.GetSocket(),F_SETFL,O_NONBLOCK);
    if(flag == -1)
    {
        printf("fcntl failed --- \n");
        return -1;
    }
	if (connect(sockClient.GetSocket(), (sockaddr*)&conAddr, sizeof(sockaddr_in)) == -1)
	{
		tm.tv_sec = timeout;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(sockClient.GetSocket(), &set);
		if (select(sockClient.GetSocket() + 1,NULL,&set,NULL, &tm) > 0)
		{
            int error;
            socklen_t len=sizeof(error);
            getsockopt(sockClient.GetSocket(),SOL_SOCKET,SO_ERROR,&error,&len); 
            if(!error)
                printf("is the ip %s -- port %d \n",conIP,conPort);    
        }
	    else
	        ret = -1;
	}


    sockClient.CloseSocket();
	return ret;
}
