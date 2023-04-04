cmake_minimum_required(VERSION 3.25)
project(blockapps)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

include_directories(images)

add_executable(blockapps main.cpp lib/MImageClass.h lib/MImageClass.cpp)
