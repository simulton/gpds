add_subdirectory(core)
add_subdirectory(serdes)

target_sources(
    ${TARGET}
    PRIVATE
        color.h

        attributes.cpp
        cdata.cpp
        container_serialization.cpp
        lists.cpp
)
