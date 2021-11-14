find_package(
    spdlog
    REQUIRED
)

find_package(
    fmt
    REQUIRED
)

set(TARGET gpds-example-spdlog)
add_executable(${TARGET})

target_sources(
    ${TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(
    ${TARGET}
    PRIVATE
        gpds-static
        spdlog::spdlog
)
