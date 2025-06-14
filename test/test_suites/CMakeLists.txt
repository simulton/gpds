add_subdirectory(core)
add_subdirectory(serdes)

target_sources(
    ${TARGET}
    PRIVATE
        color.h
)
