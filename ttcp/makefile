obj= epoll_test.o  tcp_socket.o socket_buf.o

SOCKDIR =socket/
cppstd=-std=c++11


nodelay:$(obj) nodelay.o
	g++     $(obj) nodelay.o -o nodelay

ConcurrentTest:$(obj) ConcurrentTest.o
	g++     $(obj) ConcurrentTest.o -o ConcurrentTest

neccat:$(obj) neccat.o
	g++     $(obj) neccat.o -o neccat

nc    :$(obj) nc.o
	g++     $(obj) nc.o     -o nc
ConcurrentTest.o:ConcurrentTest.cpp
	g++     -c $(cppstd) ConcurrentTest.cpp
load_test.o:load_test.cpp
	g++     -c $(cppstd) load_test.cpp
neccat.o:neccat.cpp
	g++     -c $(cppstd) neccat.cpp

nc.o:nc.cpp
	g++     -c $(cppstd) nc.cpp

nodelay.o:nodelay.cpp
	g++     -c $(cppstd) nodelay.cpp

tcp_socket.o:$(SOCKDIR)tcp_socket.cpp
	g++     -c $(cppstd) $(SOCKDIR)tcp_socket.cpp

socket_buf.o:$(SOCKDIR)socket_buf.cpp
	g++     -c $(cppstd) $(SOCKDIR)socket_buf.cpp

epoll_test.o:$(SOCKDIR)epoll_test.cpp
	g++     -c $(cppstd) $(SOCKDIR)epoll_test.cpp

.PHONY:clean
clean:
	rm *.o client load_test netcat nc
