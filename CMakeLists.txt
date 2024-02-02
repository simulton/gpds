cmake_minimum_required(VERSION 3.20)

# Project information
project(gpds
    VERSION 1.8.1
    LANGUAGES CXX
    HOMEPAGE_URL "https://gpds.simulton.com"
)

# Lets have some options! \o/
set(OPTION_BUILD_SHARED_DEFAULT ON)
if (MSVC)
    set(OPTION_BUILD_SHARED_DEFAULT OFF)
endif()
option(GPDS_BUILD_STATIC                    "Build static library." ON)
option(GPDS_BUILD_SHARED                    "Build shared library." ${OPTION_BUILD_SHARED_DEFAULT})
option(GPDS_BUILD_TESTS                     "Build the unit tests." OFF)
option(GPDS_BUILD_EXAMPLES                  "Build the examples." OFF)
option(GPDS_FEATURE_SPDLOG                  "Enable spdlog sink feature." OFF)
option(GPDS_DEPENDENCY_TINYXML2_FINDPACKAGE "Whether to use find_package() for the tinyxml2 dependency" OFF)
option(GPDS_DEPENDENCY_TINYXML2_PKGCONFIG   "Whether to use find_package(PkgConfig) for the tinyxml2 dependency" OFF)

# User settings
set(CMAKE_DEBUG_POSTFIX d)

# When using MSVC, only allow enabling either static or shared libs but not both
if (MSVC)
    if (GPDS_BUILD_STATIC AND GPDS_BUILD_SHARED)
        message(FATAL_ERROR "GPDS: Can only build either the static or the shared library but not both at the same time when using MSVC.")
    endif()
endif()

# Library target for internal usage
set(GPDS_TARGET_INTERNAL gpds-static)
if (NOT GPDS_BUILD_STATIC)
    set(GPDS_TARGET_INTERNAL gpds-shared)
endif()

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
message(STATUS "  version           : " ${PROJECT_VERSION})
message(STATUS "")
message(STATUS "  build:")
message(STATUS "    static          : " ${GPDS_BUILD_STATIC})
message(STATUS "    shared          : " ${GPDS_BUILD_SHARED})
message(STATUS "    examples        : " ${GPDS_BUILD_EXAMPLES})
message(STATUS "    tests           : " ${GPDS_BUILD_TESTS})
message(STATUS "    system tinyxml2 : " ${_SYSTEM_TINYXML2})
message(STATUS "")
message(STATUS "  misc:")
message(STATUS "    spdlog sink     : " ${GPDS_FEATURE_SPDLOG})
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
