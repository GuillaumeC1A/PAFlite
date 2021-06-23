cmake_minimum_required(VERSION 3.10.2)
project(gnob)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
add_executable(gnob
        main.cpp
        rf.cpp
        rf.h)
