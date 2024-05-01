set(TARGET gpds-tests)

add_executable(${TARGET})

add_subdirectory(test_suites)

target_sources(
	${TARGET}
	PRIVATE
		mocks.hpp
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
		${GPDS_TARGET_INTERNAL}
)

###
# CTest
###
enable_testing()
add_test(
	NAME doctest
	COMMAND ${TARGET}
)
