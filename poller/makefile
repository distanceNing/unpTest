obj=event_loop.o main.o poller.o
server:$(obj)
	 g++ $(obj) -o server 
main.o:main.cpp

poller.o:poller.cpp

event_loop.o:event_loop.cpp
clean:
	rm *.o server
