cmake_minimum_required(VERSION 3.13)
project(differential-expwy CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall")

add_executable(main src/main.cpp src/assignment.cpp src/io.cpp src/network.cpp src/od.cpp)
add_executable(test src/test_main.cpp src/assignment.cpp src/io.cpp src/network.cpp src/od.cpp)