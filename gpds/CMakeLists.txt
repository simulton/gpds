# CMake settiongs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Define directory base path for FILE_SET
set(GPDS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Dependency (tinyxml2)
if (GPDS_ENABLE_XML)
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
		set(GPDS_DEPENDENCY_TINYXML2_SOURCE_BUILD ON)
	endif()
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

	target_compile_definitions(
		${target}
		PUBLIC
			$<$<BOOL:${GPDS_ENABLE_XML}>:GPDS_ENABLE_XML>
			$<$<BOOL:${GPDS_ENABLE_YAML}>:GPDS_ENABLE_YAML>
	)

	target_include_directories(
		${target}
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
			$<INSTALL_INTERFACE:..>
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

	target_sources(
		${target}
		PUBLIC
			FILE_SET
				HEADERS
				BASE_DIRS ${GPDS_BASE_DIR}
				FILES
					archiver.hpp
					$<$<BOOL:${GPDS_ENABLE_XML}>:archiver_xml.hpp>
					$<$<BOOL:${GPDS_ENABLE_YAML}>:archiver_yaml.hpp>
					attributes.hpp
					concepts.hpp
					container.hpp
					serialize.hpp
					spdlog_sink.hpp
					utils.hpp
					value.hpp

		PRIVATE
			$<$<BOOL:${GPDS_ENABLE_XML}>:archiver_xml.cpp>
			$<$<BOOL:${GPDS_ENABLE_YAML}>:archiver_yaml.cpp>
			value.cpp
			$<$<BOOL:${GPDS_DEPENDENCY_TINYXML2_SOURCE_BUILD}>:3rdparty/tinyxml2/tinyxml2.cpp>
			$<$<BOOL:${GPDS_DEPENDENCY_TINYXML2_SOURCE_BUILD}>:3rdparty/tinyxml2/tinyxml2.h>
			$<$<BOOL:${GPDS_ENABLE_YAML}>:3rdparty/miniyaml/yaml/yaml.cpp>
			$<$<BOOL:${GPDS_ENABLE_YAML}>:3rdparty/miniyaml/yaml/yaml.hpp>
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
	FILE_SET
		HEADERS
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gpds
	LIBRARY
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT lib
	ARCHIVE
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT lib
	RUNTIME
		DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT bin
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
