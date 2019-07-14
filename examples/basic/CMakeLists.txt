# A list of our files
set(_sources main.cpp)
set(_headers car.h color.h carcatalog.h)
set(_files ${_sources} ${_headers})

# Add library
include_directories(
    ../../src/gpds
    ../../src/3rdparty/rapidxml-1.13
)

# Build the executable
add_executable(gpds_demo ${_files})
target_link_libraries(gpds_demo gpds)
