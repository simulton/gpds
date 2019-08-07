###
# Testing (using catch2)
###

# Add catch2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/3rdparty/catch2/contrib)
add_subdirectory(3rdparty/catch2)

# Find tests
FILE(GLOB TESTS tests/*.cpp)

# Build test application
add_executable(gpds-tests
	test.h
	test.cpp
	test_main.cpp
	${TESTS})
target_link_libraries(gpds-tests gpds-static)
target_link_libraries(gpds-tests Catch2::Catch2)

# Automatically discover tests
include(CTest)
include(Catch)
catch_discover_tests(gpds-tests)
