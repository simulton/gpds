# CMake settiongs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# List of private source files
set(SOURCES_PRIVATE
	src/archiver_xml.cpp
	src/archiver_yaml.cpp
	src/value.cpp
	3rdparty/miniyaml/yaml/yaml.cpp
)

# List of private header files
set(HEADERS_PRIVATE
	3rdparty/miniyaml/yaml/yaml.hpp
)

# List of public header files
set(HEADERS_PUBLIC
	include/gpds/archiver.hpp
	include/gpds/archiver_xml.hpp
	include/gpds/archiver_yaml.hpp
	include/gpds/attributes.hpp
	include/gpds/concepts.hpp
	include/gpds/container.hpp
	include/gpds/serialize.hpp
	include/gpds/spdlog_sink.hpp
	include/gpds/utils.hpp
	include/gpds/value.hpp
)

# Dependency (tinyxml2)
if (GPDS_DEPENDENCY_TINYXML2_FINDPACKAGE OR GPDS_DEPENDENCY_TINYXML2_PKGCONFIG)
	if(GPDS_DEPENDENCY_TINYXML2_FINDPACKAGE)
		find_package(tinyxml2 REQUIRED)
		set(TINYXML2_TARGET tinyxml2::tinyxml2)
	else()
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(tinyxml2 REQUIRED IMPORTED_TARGET tinyxml2)
		set(TINYXML2_TARGET PkgConfig::tinyxml2)
	endif()
else()
	list(APPEND HEADERS_PRIVATE
		3rdparty/tinyxml2/tinyxml2.h
	)
	list(APPEND SOURCES_PRIVATE
		3rdparty/tinyxml2/tinyxml2.cpp
	)
endif()

# Define targets
set(NAME gpds)
set(TARGET_STATIC ${NAME}-static)
set(TARGET_SHARED ${NAME}-shared)

# This function sets stuff up common to all targets
function(setup_target_common target)
	# Add alias to support CMake superbuild pattern
	add_library(gpds::${target} ALIAS ${target})

	target_compile_features(
		${target}
		PUBLIC
			cxx_std_20
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

	target_link_libraries(
		${target}
		PRIVATE
			$<$<OR:$<BOOL:${GPDS_DEPENDENCY_TINYXML2_FINDPACKAGE}>,$<BOOL:${GPDS_DEPENDENCY_TINYXML2_PKGCONFIG}>>:${TINYXML2_TARGET}>
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

if (GPDS_BUILD_SHARED)
	add_library(${TARGET_SHARED} SHARED)
	setup_target_common(${TARGET_SHARED})
endif()


################################################################################
# Static library                                                               #
################################################################################

if (GPDS_BUILD_STATIC)
	add_library(${TARGET_STATIC} STATIC)
	setup_target_common(${TARGET_STATIC})
endif()


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
if (GPDS_BUILD_STATIC)
	list(APPEND INSTALL_TARGETS_LIST ${TARGET_STATIC})
endif()
if (GPDS_BUILD_SHARED)
	list(APPEND INSTALL_TARGETS_LIST ${TARGET_SHARED})
endif()
install(
	TARGETS
		${INSTALL_TARGETS_LIST}
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
		"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config-version.cmake"
	DESTINATION ${ConfigPackageLocation}
)

# Package configuration
configure_file(gpds-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/gpds/gpds-config.cmake"
	@ONLY
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
