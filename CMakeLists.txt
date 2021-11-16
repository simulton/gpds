cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Project information
project(gpds
	LANGUAGES CXX
)

# Lets have some options! \o/
option(GPDS_BUILD_TESTS    "Build the unit tests." on)
option(GPDS_BUILD_EXAMPLES "Build the examples." on)
option(GPDS_FEATURE_SPDLOG "Enable spdlog sink feature." on)

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
message("")
message("------------------------")
message("GPDS configuration:")
message("  build examples : " ${GPDS_BUILD_EXAMPLES})
message("  build tests    : " ${GPDS_BUILD_TESTS})
message("  spdlog sink    : " ${GPDS_FEATURE_SPDLOG})
message("------------------------")
message("")

################################################################################
# CPack                                                                        #
################################################################################
set(CPACK_PACKAGE_VENDOR "Simulton GmbH")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A general purpose data serializer library.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/readme.md")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
