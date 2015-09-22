cmake_minimum_required(VERSION 2.8)


set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS} -g")

add_executable(Shell ShellReactor.cpp Shell.cpp ShellParser.cpp Process.cpp ShellCommand.cpp Job.cpp)
