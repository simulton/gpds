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
	include/gpds/utils.hpp
	include/gpds/value.hpp
)

# spdlog sink
if (GPDS_FEATURE_SPDLOG)
	list(APPEND HEADERS_PUBLIC
		include/gpds/spdlog_sink.hpp
	)
endif()

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
			POSITION_INDEPENDENT_CODE ON
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
