cmake_minimum_required(VERSION 3.7)
project(epoll)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp epoll_test.cpp epoll_test.h common.cpp common.h tcp_socket.cpp tcp_socket.h


        )
add_executable(epoll ${SOURCE_FILES})