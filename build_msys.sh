#!/bin/bash

# Variables
MAKE_BIN=mingw32-make.exe
MAKE_JOB_COUNT=4
CMAKE_BIN=cmake


##############################################################################
### Don't mess around beyond this point unless you know what you're doing. ###
##############################################################################


# Terminate script upon errors
set -ex

# Setup environment
mkdir build
cd build

# CMake
${CMAKE_BIN} \
	-G'MinGW Makefiles' \
	-DCMAKE_C_COMPILER=gcc \
	-DCMAKE_CXX_COMPILER=g++ \
	..

# Build
${MAKE_BIN} -j${MAKE_JOB_COUNT}

