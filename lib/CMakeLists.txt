set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of source files
set(SOURCES
	src/archiverxml.cpp
	src/container.cpp
	src/value.cpp)

# List of header files
set(HEADERS
	include/gpds/archiver.h
	include/gpds/archiverxml.h
	include/gpds/attributes.h
	include/gpds/container.h
	include/gpds/serialize.h
	include/gpds/types.h
	include/gpds/utils.h
	include/gpds/value.h)

# List of header + source files
set(FILES ${SOURCES} ${HEADERS})

# Include directories
include_directories(
	include
	3rdparty/rapidxml-1.13)


# Generate objects library
add_library(gpds-objs OBJECT ${FILES})
# this is only needed for older compilers
# but doesn't hurt either to have it
set_target_properties(gpds-objs
	PROPERTIES
		POSITION_INDEPENDENT_CODE 1)

# Generate shared library
add_library(gpds-shared SHARED $<TARGET_OBJECTS:gpds-objs>)
set_target_properties(gpds-shared
	PROPERTIES
		OUTPUT_NAME "gpds")

# Generate static library
add_library(gpds-static STATIC $<TARGET_OBJECTS:gpds-objs>)
set_target_properties(gpds-static
	PROPERTIES
		OUTPUT_NAME "gpds")

###
# Install
###

# Copy libraries
install(TARGETS gpds-static DESTINATION gpds/lib)
install(TARGETS gpds-shared DESTINATION gpds/lib)

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION gpds/include/gpds)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION gpds/include/gpds
	FILES_MATCHING PATTERN "*.hpp")

