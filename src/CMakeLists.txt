set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of source files
set(SOURCES src/value.cpp)

# List of header files
set(HEADERS
	gpds/archiver.h
	gpds/archiverxml.h
	gpds/attributes.h
	gpds/container.h
	gpds/serialize.h
	gpds/types.h
	gpds/utils.h
	gpds/value.h)

# List of header + source files
set(FILES ${SOURCES} ${HEADERS})

# Include directories
include_directories(
	.
	3rdparty/rapidxml-1.13)

# Generate objects library
add_library(gpds-objs OBJECT ${FILES})

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
install(TARGETS gpds-static DESTINATION lib)
install(TARGETS gpds-shared DESTINATION lib)

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION gpds)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION gpds
	FILES_MATCHING PATTERN "*.hpp")

