# Set project information
project(gpds
	VERSION 0.1.0
	LANGUAGES CXX
	HOMEPAGE_URL "https://gpds.simulton.com")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of private source files
set(SOURCES_PRIVATE
	src/archiverxml.cpp
	src/container.cpp
	src/value.cpp)

# List of public header files
set(HEADERS_PUBLIC
	include/archiver.h
	include/archiverxml.h
	include/attributes.h
	include/container.h
	include/serialize.h
	include/types.h
	include/utils.h
	include/value.h)

# Generate objects library
add_library(gpds-objs OBJECT "")
set_target_properties(gpds-objs
	PROPERTIES
		POSITION_INDEPENDENT_CODE 1)
target_compile_features(gpds-objs
	PUBLIC
		cxx_std_17)
target_sources(gpds-objs
	PUBLIC
		${HEADERS_PUBLIC}
	PRIVATE
		${SOURCES_PRIVATE})
target_include_directories(gpds-objs
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rapidxml-1.13>
	INTERFACE
		$<INSTALL_INTERFACE:gpds/include>)

###
# Shared library
###
add_library(gpds-shared SHARED $<TARGET_OBJECTS:gpds-objs>)
set_target_properties(gpds-shared
	PROPERTIES
		OUTPUT_NAME "gpds"
		VERSION ${PROJECT_VERSION}
		POSITION_INDEPENDENT_CODE 1)

###
# Static library
###
add_library(gpds-static STATIC $<TARGET_OBJECTS:gpds-objs>)
target_compile_definitions(gpds-static
	PUBLIC
		GPDS_STATIC_DEFINE
)
set_target_properties(gpds-static
	PROPERTIES
		ARCHIVE_OUTPUT_NAME "gpds"
		VERSION ${PROJECT_VERSION}
		POSITION_INDEPENDENT_CODE 1
)

###
# Export header
###

include(GenerateExportHeader)
generate_export_header(gpds-static
	BASE_NAME gpds
	DEPRECATED_MACRO_NAME "GPDS_DEPRECATED"
	NO_DEPRECATED_MACRO_NAME "GPDS_NO_DEPRECATED"
	EXPORT_MACRO_NAME "GPDS_EXPORT"
	NO_EXPORT_MACRO_NAME "GPDS_NO_EXPORT"
	STATIC_DEFINE "GPDS_STATIC_DEFINE"
	DEFINE_NO_DEPRECATED)

###
# Install
###

include(GNUInstallDirs)

# Copy GPDS headers
install(
	FILES ${HEADERS_PUBLIC}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds)

# Install export header
install(FILES
	${PROJECT_BINARY_DIR}/gpds_export.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds)

# Copy rapidxml headers
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/3rdparty/rapidxml-1.13/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
	FILES_MATCHING PATTERN "*.hpp")

# Targets
install(
	TARGETS
		gpds-shared
		gpds-static
	EXPORT GpdsTargets
	LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT lib
	ARCHIVE
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT lib
	RUNTIME
		DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT bin
	INCLUDES
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
		COMPONENT dev)

###
# Packging
###
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

set(ConfigPackageLocation lib/cmake/Gpds)
install(
	EXPORT GpdsTargets
	FILE GpdsTargets.cmake
	NAMESPACE Gpds::
	DESTINATION ${ConfigPackageLocation}
)

install(
	FILES
		GpdsConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfigVersion.cmake"
	DESTINATION ${ConfigPackageLocation}
)
