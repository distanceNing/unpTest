obj=client.o time_stamp.o tcp_socket.o socket_buf.o

SOCKDIR =socket/
cppstd=-std=c++11

client:$(obj)
	g++     $(obj) -o client

client.o:client.cpp
	g++     -c $(cppstd) client.cpp

time_stamp.o:time_stamp.cpp
	g++     -c $(cppstd) time_stamp.cpp

tcp_socket.o:$(SOCKDIR)tcp_socket.cpp
	g++     -c $(cppstd) $(SOCKDIR)tcp_socket.cpp

socket_buf.o:$(SOCKDIR)socket_buf.cpp
	g++     -c $(cppstd) $(SOCKDIR)socket_buf.cpp

.PHONY:clean
clean:
	rm *.o client
