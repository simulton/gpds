###
# Testing (using catch2)
###

# Add catch2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/3rdparty/catch2/contrib)
add_subdirectory(3rdparty/catch2)

# Build test application
add_executable(gpds-tests
	test.h
	test.cpp
	test_main.cpp
	tests/test_01.cpp
	tests/test_02.cpp
	tests/test_03.cpp
	tests/test_04.cpp
	tests/test_05.cpp
	tests/test_06.cpp
	tests/test_07.cpp)
target_link_libraries(gpds-tests gpds-static)
target_link_libraries(gpds-tests Catch2::Catch2)

# Automatically discover tests
include(CTest)
include(Catch)
catch_discover_tests(gpds-tests)
