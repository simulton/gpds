set(TARGET gpds-example-basic)

add_executable(${TARGET})

target_sources(
    ${TARGET}
    PRIVATE
        main.cpp
        car.h
        color.h
        carcatalog.h
)

target_link_libraries(
    ${TARGET}
    PRIVATE
        ${GPDS_TARGET_INTERNAL}
)
