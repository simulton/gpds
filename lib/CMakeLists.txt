# Set project information
project(gpds
	VERSION 0.1.0
	LANGUAGES CXX
	HOMEPAGE_URL "https://gpds.simulton.com")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
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

# Generate objects library
add_library(gpds-objs OBJECT ${FILES})
# this is only needed for older compilers
# but doesn't hurt either to have it
set_target_properties(gpds-objs
	PROPERTIES
		POSITION_INDEPENDENT_CODE 1)
target_compile_features(gpds-objs
	PUBLIC
		cxx_std_17)

# Generate static library
add_library(gpds-static STATIC $<TARGET_OBJECTS:gpds-objs>)
set_target_properties(gpds-static
	PROPERTIES
		OUTPUT_NAME "gpds"
		VERSION ${PROJECT_VERSION})

# Export
include(GenerateExportHeader)
generate_export_header(gpds-static
	BASE_NAME gpds)

# Include directories
target_include_directories(gpds-objs
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rapidxml-1.13>
	INTERFACE
		$<INSTALL_INTERFACE:gpds/include>)

###
# Install
###

# Copy GPDS headers
install(FILES ${HEADERS} DESTINATION gpds/include/gpds)

# Install export header
install(FILES
	${PROJECT_BINARY_DIR}/gpds_export.h
	DESTINATION gpds/include/gpds)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION gpds/include/gpds
	FILES_MATCHING PATTERN "*.hpp")

# Copy static library
install(TARGETS gpds-static EXPORT GpdsTargets
		LIBRARY DESTINATION gpds/lib
		ARCHIVE DESTINATION gpds/lib
		RUNTIME DESTINATION gpds/bin
		INCLUDES DESTINATION gpds/include/gpds
		)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfigVersion.cmake"
		VERSION ${Gpds_VERSION}
		COMPATIBILITY AnyNewerVersion
)

export(EXPORT GpdsTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsTargets.cmake"
		NAMESPACE Gpds::
		)
configure_file(GpdsConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfig.cmake"
		COPYONLY
		)

set(ConfigPackageLocation gpds/lib/cmake/Gpds)
install(EXPORT GpdsTargets
		FILE
		GpdsTargets.cmake
		NAMESPACE
		Gpds::
		DESTINATION
		${ConfigPackageLocation}
		)
install(
		FILES
		GpdsConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfigVersion.cmake"
		DESTINATION
		${ConfigPackageLocation}
)
