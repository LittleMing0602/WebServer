project(demo)
cmake_minimum_required(VERSION 2.6)
add_subdirectory(net)
add_subdirectory(base)

add_executable(EventLoop_test1 test/EventLoop_test1.cc)
target_link_libraries(EventLoop_test1 Thread net pthread)

add_executable(EventLoop_test2 test/EventLoop_test2.cc)
target_link_libraries(EventLoop_test2 Thread net pthread)

add_executable(EventLoop_test3 test/EventLoop_test3.cc)
target_link_libraries(EventLoop_test3 net)
