# Traffic Control System

This project implements a smart traffic control system using SFML (Simple and Fast Multimedia Library). The system manages traffic lights, vehicles, and pedestrian signals to simulate real-time traffic management.

## Project Structure

- `src/traffic_system.cpp`: Contains the implementation of the traffic control system.
- `include`: Intended for header files (currently empty).
- `CMakeLists.txt`: Configuration file for CMake to build the project.
- `README.md`: Documentation for the project.

## Requirements

- CMake
- A C++ compiler (e.g., MinGW or Visual Studio)
- SFML library

## Setup Instructions

1. **Install CMake** and a C++ compiler (like MinGW or Visual Studio).
2. **Install the SFML library** and ensure it is accessible in your project.
3. **Open a terminal** and navigate to the project directory.
4. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```
5. **Run CMake** to configure the project:
   ```bash
   cmake ..
   ```
6. **Build the project**:
   ```bash
   cmake --build .
   ```
7. **Run the executable** generated in the build directory.

## Usage

After building the project, you can run the executable to start the traffic control system simulation. Use the keyboard controls to interact with the system, manage pedestrian requests, and observe the traffic light changes.