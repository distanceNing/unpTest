cmake_minimum_required(VERSION 3.7)
project(poller)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        channel.cpp
        channel.h
        event_loop.cpp
        event_loop.h
        main.cpp
        poller.cpp
        poller.h
        print_time.h
        tcp_socket.cpp
        tcp_socket.h
        timer.h common.cpp common.h)

add_executable(poller ${SOURCE_FILES})