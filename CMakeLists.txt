project(demo)
cmake_minimum_required(VERSION 2.6)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -g")
endif()

add_subdirectory(net)
add_subdirectory(base)
add_subdirectory(timer)
add_subdirectory(log)
add_subdirectory(mysql)
add_subdirectory(server)
add_subdirectory(lock)

# include_directories(log)

add_executable(EventLoop_test1 test/EventLoop_test1.cc)
target_link_libraries(EventLoop_test1 timer thread net)

add_executable(EventLoop_test2 test/EventLoop_test2.cc)
target_link_libraries(EventLoop_test2 timer thread net)

add_executable(EventLoop_test3 test/EventLoop_test3.cc)
target_link_libraries(EventLoop_test3 net timer thread)

add_executable(Acceptor_test1 test/Acceptor_test1.cc)
target_link_libraries(Acceptor_test1 net timer thread)

add_executable(TcpConnection_test1 test/TcpConnection_test1.cc)
target_link_libraries(TcpConnection_test1 timer net thread)

add_executable(EchoServer test/EchoServer.cc)
target_link_libraries(EchoServer net timer thread)

add_executable(Send_test test/Send_Test.cc)
target_link_libraries(Send_test net timer thread)

add_executable(EventLoopThreadPool_test test/EventLoopThreadPool_test.cc)
target_link_libraries(EventLoopThreadPool_test net thread timer)

add_executable(HttpServer_test test/HttpServer_test.cc)
target_link_libraries(HttpServer_test http log)

add_executable(MysqlConnection_test test/MysqlConnection_test.cc)
target_link_libraries(MysqlConnection_test mysql)

add_executable(WebServer_test test/WebServer_test.cc)
target_link_libraries(WebServer_test webserver)

add_executable(AsyncLogging_test test/AsyncLogging_test.cc)
target_link_libraries(AsyncLogging_test log lock)
