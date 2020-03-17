###
# Testing (using doctest)
###

# Add doctest
configure_file(CMakeLists.txt.in doctest/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doctest )
if (result)
	message(FATAL_ERROR "CMake step for doctest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doctest )
if (result)
	message(FATAL_ERROR "Build step for doctest failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/doctest-src
		         ${CMAKE_CURRENT_BINARY_DIR}/doctest-build
			     EXCLUDE_FROM_ALL)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/doctest-src/doctest)

include_directories(../lib/3rdparty/tinyxml2)
# Find tests
FILE(GLOB TESTS tests/*.cpp)

# Build test application
add_executable(gpds-tests
	test.h
	test.cpp
	test_main.cpp
	${TESTS})
#add_dependencies(gpds-tests gpds-static)
target_link_libraries(gpds-tests gpds-static)

# Automatically discover tests
include(CTest)
include(${CMAKE_CURRENT_BINARY_DIR}/doctest-src/scripts/cmake/doctest.cmake)
doctest_discover_tests(gpds-tests)
