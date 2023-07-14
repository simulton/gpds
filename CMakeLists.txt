cmake_minimum_required(VERSION 3.20)

# Project information
project(gpds
	VERSION 1.6.0
	LANGUAGES CXX
	HOMEPAGE_URL "https://gpds.simulton.com"
)

# Lets have some options! \o/
option(GPDS_BUILD_TESTS    "Build the unit tests." OFF)
option(GPDS_BUILD_EXAMPLES "Build the examples." OFF)
option(GPDS_FEATURE_SPDLOG "Enable spdlog sink feature." ON)

# User settings
set(CMAKE_DEBUG_POSTFIX d)

# Build the actual library
add_subdirectory(lib)

# Testing
if (GPDS_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()

# Examples
if (GPDS_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()


################################################################################
# Print configuration
################################################################################
message(STATUS "")
message(STATUS "------------------------")
message(STATUS "GPDS configuration:")
message(STATUS "  build examples : " ${GPDS_BUILD_EXAMPLES})
message(STATUS "  build tests    : " ${GPDS_BUILD_TESTS})
message(STATUS "  spdlog sink    : " ${GPDS_FEATURE_SPDLOG})
message(STATUS "------------------------")
message(STATUS "")

################################################################################
# CPack                                                                        #
################################################################################
set(CPACK_PACKAGE_VENDOR "Simulton GmbH")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A general purpose data serializer library.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/readme.md")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
