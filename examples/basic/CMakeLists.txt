cmake_minimum_required(VERSION 3.13)

project(gpds_demo)

set(CMAKE_CXX_STANDARD 17)

# A list of our files
set(_sources main.cpp)
set(_headers car.h color.h carcatalog.h)
set(_files ${_sources} ${_headers})

# Build th eexecutable
add_executable(demo_app ${_files})
target_link_libraries(demo_app gpds)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
