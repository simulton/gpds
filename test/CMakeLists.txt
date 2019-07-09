###
# Testing (using catch2)
###

include_directories(
	../src/include
	../src/3rdparty/rapidxml-1.13)

# Build test application
add_executable(
	gpds_test
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
