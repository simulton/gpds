# A list of our files
set(_sources main.cpp)
set(_headers car.h color.h carcatalog.h)
set(_files ${_sources} ${_headers})

# Add library
include_directories(
    ../../lib/include
    ../../lib/3rdparty/rapidxml-1.13
)

# Build the executable
add_executable(gpds-example ${_files})
target_link_libraries(gpds-example $<TARGET_OBJECTS:gpds-objs>)
