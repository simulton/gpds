cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project information
project(gpds
	LANGUAGES CXX
	VERSION 0.1.0)

# Lets have some options! \o/
option(GPDS_BUILD_TESTS "Build the unit tests" on)
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
if (GPDS_BUILD_TESTS OR true)
	enable_testing()
	add_subdirectory(test)
endif()

# Examples
if (GPDS_BUILD_EXAMPLES OR true)
	add_subdirectory(examples/basic)
endif()
