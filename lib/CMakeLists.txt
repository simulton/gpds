# Set project information
project(gpds
	VERSION 0.1.0
	LANGUAGES CXX
	HOMEPAGE_URL "https://gpds.simulton.com")

# Some bacis cmake configuration
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of private source files
set(SOURCES_PRIVATE
	src/archiverxml.cpp
	src/container.cpp
	src/value.cpp
	src/attributes.cpp
	3rdparty/tinyxml2/tinyxml2.cpp)

# List of private header files
set(HEADERS_PRIVATE
	3rdparty/tinyxml2/tinyxml2.h)

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



################################################################################
# Object library                                                               #
################################################################################

add_library(gpds-objs OBJECT "")
target_compile_features(gpds-objs
	PUBLIC
		cxx_std_17)
target_sources(gpds-objs
	PUBLIC
		${HEADERS_PUBLIC}
	PRIVATE
		${SOURCES_PRIVATE}
		${HEADERS_PRIVATE})
target_compile_definitions(gpds-objs
	PUBLIC
		gpds_shared_EXPORTS		# We're building this library!
)
target_include_directories(gpds-objs
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	INTERFACE
		$<INSTALL_INTERFACE:include>)



################################################################################
# Shared library                                                               #
################################################################################

add_library(gpds-shared SHARED $<TARGET_OBJECTS:gpds-objs>)
target_include_directories(
	gpds-shared
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	INTERFACE
		$<INSTALL_INTERFACE:include>
)



################################################################################
# Static library                                                               #
################################################################################

add_library(gpds-static STATIC $<TARGET_OBJECTS:gpds-objs>)
target_compile_definitions(gpds-static
	PUBLIC
		GPDS_STATIC_DEFINE
)
target_include_directories(
	gpds-static
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	INTERFACE
		$<INSTALL_INTERFACE:include>
)

# Common library properties
set_target_properties(
	gpds-objs
	gpds-static
	gpds-shared
	PROPERTIES
		OUTPUT_NAME "gpds"
		ARCHIVE_OUTPUT_NAME "gpds"
		VERSION ${PROJECT_VERSION}
		POSITION_INDEPENDENT_CODE 1
)



################################################################################
# Export header                                                                #
################################################################################

include(GenerateExportHeader)
generate_export_header(gpds-shared
	BASE_NAME gpds
	DEPRECATED_MACRO_NAME "GPDS_DEPRECATED"
	NO_DEPRECATED_MACRO_NAME "GPDS_NO_DEPRECATED"
	EXPORT_MACRO_NAME "GPDS_EXPORT"
	NO_EXPORT_MACRO_NAME "GPDS_NO_EXPORT"
	STATIC_DEFINE "GPDS_STATIC_DEFINE"
	DEFINE_NO_DEPRECATED)



################################################################################
# Install                                                                      #
################################################################################

# Some basic stuff we'll need in this section
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/Gpds)

# Copy GPDS headers
install(
	FILES ${HEADERS_PUBLIC}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
)

# Install export header
install(
	FILES ${PROJECT_BINARY_DIR}/gpds_export.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
)

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
)

# Package version
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfigVersion.cmake"
	VERSION ${Gpds_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(
	FILES
		GpdsConfig.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfigVersion.cmake"
		DESTINATION ${ConfigPackageLocation}
)

# Package configuration
configure_file(GpdsConfig.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsConfig.cmake"
	COPYONLY
)

# Package export targets
export(
	EXPORT GpdsTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/Gpds/GpdsTargets.cmake"
	NAMESPACE gpds::
)
install(
	EXPORT GpdsTargets
	FILE GpdsTargets.cmake
	NAMESPACE Gpds::
	DESTINATION ${ConfigPackageLocation}
)
