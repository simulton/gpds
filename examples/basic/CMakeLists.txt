# A list of our files
set(SOURCES main.cpp)
set(HEADERS car.h color.h carcatalog.h)

# Build the executable
add_executable(gpds-example "")
add_dependencies(gpds-example gpds-static)
target_sources(gpds-example
    PRIVATE
        ${SOURCES}
        ${HEADERS}
)
target_link_libraries(gpds-example gpds-static)
