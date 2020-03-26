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
	src/archiver_xml.cpp
	src/container.cpp
	src/value.cpp
	src/attributes.cpp
	src/serialize.cpp
	3rdparty/tinyxml2/tinyxml2.cpp)

# List of private header files
set(HEADERS_PRIVATE
	3rdparty/tinyxml2/tinyxml2.h
)

# Include tinyxml2
include_directories(PUBLIC 3rdparty/tinyxml2)

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

if (GPDS_FEATURE_XPATH)
	set(HEADERS_PUBLIC ${HEADERS_PUBLIC}
		include/gpds/document.hpp
		include/gpds/document_xml.hpp
		include/gpds/fragment.hpp
		include/gpds/fragment_xml.hpp
	)

	set(HEADAERS_PRIVATE ${HEADERS_PRIVATE}
		3rdparty/tinyxml2-ex/tixml2ex.h
	)

	set(SOURCES_PRIVATE ${SOURCES_PRIVATE}
		src/document_xml.cpp
		src/fragment_xml.cpp
	)
endif()



################################################################################
# Object library                                                               #
################################################################################

add_library(gpds-objs OBJECT "")
target_compile_features(gpds-objs
	PUBLIC
		cxx_std_17)
target_sources(gpds-objs
	PRIVATE
		${SOURCES_PRIVATE}
		${HEADERS_PRIVATE}
)
target_compile_definitions(gpds-objs
	PUBLIC
		gpds_shared_EXPORTS		# We're building this library!
		$<$<BOOL:${GPDS_FEATURE_XPATH}>:GPDS_FEATURE_XPATH>
)
target_include_directories(gpds-objs
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/gpds>
)



################################################################################
# Shared library                                                               #
################################################################################

add_library(gpds-shared SHARED)
target_link_libraries(gpds-shared PUBLIC gpds-objs)



################################################################################
# Static library                                                               #
################################################################################

add_library(gpds-static STATIC)
target_link_libraries(gpds-static PUBLIC gpds-objs)

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
	EXPORT_FILE_NAME gpds_export.hpp
	DEPRECATED_MACRO_NAME "GPDS_DEPRECATED"
	NO_DEPRECATED_MACRO_NAME "GPDS_NO_DEPRECATED"
	EXPORT_MACRO_NAME "GPDS_EXPORT"
	NO_EXPORT_MACRO_NAME "GPDS_NO_EXPORT"
	STATIC_DEFINE "GPDS_STATIC_DEFINE"
	DEFINE_NO_DEPRECATED
)



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
		gpds-shared
		gpds-static
		gpds-objs
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
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
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
