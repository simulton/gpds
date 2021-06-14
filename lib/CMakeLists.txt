# Set project information
project(gpds
	VERSION 1.1.0
	LANGUAGES CXX
	HOMEPAGE_URL "https://gpds.simulton.com"
)

# Some bacis cmake configuration
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# List of private source files
set(SOURCES_PRIVATE
	src/archiver_xml.cpp
	src/container.cpp
	src/value.cpp
	src/serialize.cpp
	3rdparty/tinyxml2/tinyxml2.cpp
)

# List of private header files
set(HEADERS_PRIVATE
	3rdparty/tinyxml2/tinyxml2.h
)

# List of public header files
set(HEADERS_PUBLIC
	include/gpds/archiver.hpp
	include/gpds/archiver_xml.hpp
	include/gpds/attributes.hpp
	include/gpds/container.hpp
	include/gpds/serialize.hpp
	include/gpds/types.hpp
	include/gpds/utils.hpp
	include/gpds/value.hpp
)

# Define targets
set(NAME gpds)
set(TARGET-STATIC ${NAME}-static)
set(TARGET-SHARED ${NAME}-shared)

# This function sets stuff up common to all targets
function(setup_target_common target)
	target_compile_features(
		${target}
		PUBLIC
			cxx_std_17
	)

	target_sources(
		${target}
		PRIVATE
			${SOURCES_PRIVATE}
			${HEADERS_PRIVATE}
	)

	target_compile_definitions(
		${target}
		PRIVATE
			gpds_shared_EXPORTS		# We're building this library!
	)

	target_include_directories(
		${target}
		INTERFACE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
			PRIVATE
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/gpds>
	)

	set_target_properties(
		${target}
		PROPERTIES
			OUTPUT_NAME "gpds"
			ARCHIVE_OUTPUT_NAME "gpds"
			VERSION ${PROJECT_VERSION}
			POSITION_INDEPENDENT_CODE 1
	)

	include(GenerateExportHeader)
	generate_export_header(
		${target}
		BASE_NAME gpds
		EXPORT_FILE_NAME gpds_export.hpp
		DEPRECATED_MACRO_NAME "GPDS_DEPRECATED"
		NO_DEPRECATED_MACRO_NAME "GPDS_NO_DEPRECATED"
		EXPORT_MACRO_NAME "GPDS_EXPORT"
		NO_EXPORT_MACRO_NAME "GPDS_NO_EXPORT"
		STATIC_DEFINE "GPDS_STATIC_DEFINE"
		DEFINE_NO_DEPRECATED
	)
endfunction()


################################################################################
# Shared library                                                               #
################################################################################

add_library(${TARGET-SHARED} SHARED)
setup_target_common(${TARGET-SHARED})


################################################################################
# Static library                                                               #
################################################################################

add_library(${TARGET-STATIC} STATIC)
setup_target_common(${TARGET-STATIC})


################################################################################
# Install                                                                      #
################################################################################

# Some basic stuff we'll need in this section
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/gpds)

# Copy GPDS headers
install(
	FILES ${HEADERS_PUBLIC}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
)

# Install export header
install(
	FILES ${PROJECT_BINARY_DIR}/gpds_export.hpp
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
)

# Targets
install(
	TARGETS
		${TARGET-OBJS}
		${TARGET-STATIC}
		${TARGET-SHARED}
	EXPORT gpds-targets
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
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Package version
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config-version.cmake"
	VERSION ${Gpds_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(
	FILES
		gpds-config.cmake
		"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config-version.cmake"
		DESTINATION ${ConfigPackageLocation}
)

# Package configuration
configure_file(gpds-config.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config.cmake"
	COPYONLY
)

# Package export targets
export(
	EXPORT gpds-targets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-targets.cmake"
	NAMESPACE gpds::
)
install(
	EXPORT gpds-targets
	FILE gpds-targets.cmake
	NAMESPACE gpds::
	DESTINATION ${ConfigPackageLocation}
)
