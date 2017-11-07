#include "socket_buf.h"
#include <string>
int main()
{
	net::SocketBuf buf,buf2;
	char* temp = "hello world";
	char buffer[1024] = { '\0' };
	std::string str(2048, '\a');
	buf.writeBytes(str.data(), str.size());
	buf.writeBytes(temp, strlen(temp));
	buf.readBytes(buffer, 10);
	buf.readBytes(buf2, 1);
	buf.writeBytes(buf2, 1);
	return 0;
}