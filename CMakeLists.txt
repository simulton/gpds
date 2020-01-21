cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Project information
project(gpds
	LANGUAGES CXX
	VERSION 0.1.0)

# Lets have some options! \o/
option(GPDS_BUILD_TESTS "Build the unit tests" on)
option(GPDS_BUILD_EXAMPLES "Build the examples" on)
option(GPDS_FEATURE_XPATH "Build with XPath support" off)

# User settings
set(CMAKE_DEBUG_POSTFIX d)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Build the actual library
add_subdirectory(lib)

# Testing
if (GPDS_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test EXCLUDE_FROM_ALL)
endif()

# Examples
if (GPDS_BUILD_EXAMPLES)
	add_subdirectory(examples/basic)
endif()
