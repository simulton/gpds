#!/bin/bash

# Variables
BUILD_PARALLEL_JOB_COUNT=4
CMAKE_BIN=cmake


##############################################################################
### Don't mess around beyond this point unless you know what you're doing. ###
##############################################################################


# Terminate script upon errors
set -ex

# Create build directory
mkdir build

# CMake
${CMAKE_BIN} \
	-B build \
	-S . \
	-G'MinGW Makefiles' \
	-DCMAKE_C_COMPILER=gcc \
	-DCMAKE_CXX_COMPILER=g++

# Build
${CMAKE_BIN} \
	--build build \
	--parallel ${BUILD_PARALLEL_JOB_COUNT}
