cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Default options
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/install)

# Project information
project(gpds
	LANGUAGES CXX
	VERSION 0.1.0)

# Lets have some options! \o/
option(GPDS_BUILD_EXAMPLES "Build the examples" off)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Build the actual library
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(test)

# Examples
if (GPDS_BUILD_EXAMPLES)
	add_subdirectory(examples/basic)
endif()
