#CMake 最低版本号要求
#cmake_minimum_required(VERSION 2.8)

# 项目信息 名称
#project(Demo)

# 查找当前目录下的所有文件，
# 并将文件保存到dir_srcs中
aux_source_directory(. dir_srcs)

#set(dir_srcs EventLoop.cc CurrentThread.cc)

add_library(net ${dir_srcs})
