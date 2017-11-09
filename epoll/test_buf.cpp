#include "socket_buf.h"
#include <string>
int main()
{
	net::SocketBuf buf,buf2;
	char* temp = "hello world lizhichao shi shabi";
	char buffer[1024] = { '\0' };
	std::string str(1024, 'a');
	buf.append(str);
	
	buf.read(buffer, 1023);
	buf.write(temp, strlen(temp));
	buf.read(buffer, 23);
	int32_t num = buf.readInt32();
	return 0;
}