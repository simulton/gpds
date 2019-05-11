cmake_minimum_required(VERSION 3.13)
project(basic)

set(CMAKE_CXX_STANDARD 17)

add_executable(basic main.cpp car.h color.h carcatalog.h ../../lib/value.cpp)

include_directories(../..l/lib)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
