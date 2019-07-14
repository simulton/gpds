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
include_directories(.
	3rdparty/rapidxml-1.13)

# Generate shared library
add_library(gpds SHARED ${FILES})


###
# Install
###

# Copy library
install(TARGETS gpds DESTINATION lib)

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION gpds)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION gpds
	FILES_MATCHING PATTERN "*.hpp")

# Copy single-inclusion-header
install(FILES ${CMAKE_CURRENT_LIST_DIR}/gpds/gpds.h
		DESTINATION gpds)
