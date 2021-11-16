set(TARGET gpds-tests)

add_executable(${TARGET})

add_subdirectory(test_suites)

target_sources(
	${TARGET}
	PRIVATE
		test.cpp
		test.hpp
		test_main.cpp
)

target_include_directories(
	${TARGET}
	PRIVATE
		3rdparty/doctest
)

target_link_libraries(
	${TARGET}
	PRIVATE
		gpds-static
)

###
# CTest
###
enable_testing()
add_test(
	NAME doctest
	COMMAND ${TARGET}
)
