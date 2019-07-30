# A list of our files
set(SOURCES main.cpp)
set(HEADERS car.h color.h carcatalog.h)

# Build the executable
add_executable(gpds-example "")
target_sources(gpds-example
    PRIVATE
        ${SOURCES}
        ${HEADERS}
)
target_link_libraries(gpds-example gpds-static)
