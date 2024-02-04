find_package(
    spdlog
    QUIET
)

add_subdirectory(basic)

if (spdlog_FOUND)
    add_subdirectory(spdlog)
endif()
