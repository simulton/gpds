find_package(
    spdlog
    QUIET
)

add_subdirectory(basic)

if (spdlog_FOUND AND GPDS_ENABLE_XML)
    add_subdirectory(spdlog)
endif()
