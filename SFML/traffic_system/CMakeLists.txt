cmake_minimum_required(VERSION 3.10)

project(TrafficSystem)

set(CMAKE_CXX_STANDARD 11)

# Specify the path to the SFML library
set(SFML_DIR "C:/path/to/SFML/lib/cmake/SFML")

# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/traffic_system.cpp
)

# Create the executable
add_executable(TrafficSystem ${SOURCES})

# Link the SFML libraries
target_link_libraries(TrafficSystem sfml-graphics sfml-window sfml-system)