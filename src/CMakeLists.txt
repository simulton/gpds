set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of source files
set(SOURCES src/value.cpp)

# List of header files
set(HEADERS
	include/archiver.h
	include/archiverxml.h
	include/attributes.h
	include/container.h
	include/serialize.h
	include/types.h
	include/utils.h
	include/value.h)

# List of header + source files
set(FILES ${SOURCES} ${HEADERS})

# Include directories
include_directories(include
	3rdparty/rapidxml-1.13)

# Generate shared library
add_library(gpds SHARED ${FILES})


###
# Install
###

# Copy library
install(TARGETS gpds DESTINATION lib)

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION include)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION include
	FILES_MATCHING PATTERN "*.hpp")

# Copy single-inclusion-header
install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/gpds.h
		DESTINATION include)
