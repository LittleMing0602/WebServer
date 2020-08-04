project(demo)
cmake_minimum_required(VERSION 2.6)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -g")
endif()

add_subdirectory(net)
add_subdirectory(base)
add_subdirectory(timer)

add_executable(EventLoop_test1 test/EventLoop_test1.cc)
target_link_libraries(EventLoop_test1 timer Thread net pthread)

add_executable(EventLoop_test2 test/EventLoop_test2.cc)
target_link_libraries(EventLoop_test2 timer Thread net pthread)

add_executable(EventLoop_test3 test/EventLoop_test3.cc)
target_link_libraries(EventLoop_test3 net timer)

add_executable(Acceptor_test1 test/Acceptor_test1.cc)
target_link_libraries(Acceptor_test1 net timer)

add_executable(TcpConnection_test1 test/TcpConnection_test1.cc)
target_link_libraries(TcpConnection_test1 timer net)
