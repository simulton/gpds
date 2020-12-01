include_directories(../lib/3rdparty/tinyxml2)

# Find tests
FILE(GLOB TESTS tests/*.cpp)

###
# Build test application
###
set(TARGET gpds-tests)

add_executable(${TARGET})

target_sources(
	${TARGET}
	PRIVATE
		test.h
		test.cpp
		test_main.cpp
		${TESTS}
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

# Automatically discover tests
include(CTest)
