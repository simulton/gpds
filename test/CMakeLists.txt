###
# Testing (using catch2)
###

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/3rdparty/catch2/contrib)

# Add catch2
add_subdirectory(3rdparty/catch2)

include_directories(
	../src/gpds
	../src/3rdparty/rapidxml-1.13
	3rdparty/catch2/single_include)

# Build test application
add_executable(
	gpds_tests
	test.h
	test_main.cpp
	tests/test_01.cpp
	tests/test_02.cpp
	tests/test_03.cpp)
target_link_libraries(gpds_tests gpds)
target_link_libraries(gpds_tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(
	gpds_tests
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/test)
