cmake_minimum_required(VERSION 3.10.2)
project(gnob)

set(CMAKE_CXX_STANDARD 14)



include_directories(.)
find_package(UHD 3.0.0 REQUIRED)
add_executable(gnob
        main.cpp
        rf.cpp
        rf.h)

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${UHD_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(gnob ${UHD_LIBRARIES} ${Boost_LIBRARIES})