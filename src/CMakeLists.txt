cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Default options
set(CMAKE_INSTALL_PREFIX ../install)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Project information
project(gpds
	LANGUAGES CXX
	VERSION 0.1.0)

# List of source files
set(SOURCES src/value.cpp)

# List of header files
set(HEADERS
	include/archiver.h
	include/archiverxml.h
	include/attributes.h
	include/container.h
	include/serialize.h
	include/types.h
	include/utils.h
	include/value.h)

# List of header + source files
set(FILES ${SOURCES} ${HEADERS})

# Include directories
include_directories(include
	3rdparty/rapidxml-1.13)

# Generate shared library
add_library(gpds SHARED ${FILES})


###
# Install
###

# Copy library
install(TARGETS gpds DESTINATION lib)

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION include)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION include
	FILES_MATCHING PATTERN "*.hpp")

# Copy single-inclusion-header
install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/gpds.h
		DESTINATION include)


###
# Testing (using catch2)
###

# Build test application
add_executable(gpds_test
		../test/test.h
		../test/test_main.cpp
		../test/tests/test_01.h
		../test/tests/test_02.h)
target_link_libraries(gpds_test gpds)

# Enable testing
enable_testing()

# Define test
add_test(
	NAME catch2_test
	COMMAND $<TARGET_FILE:gpds_test> --success
)
