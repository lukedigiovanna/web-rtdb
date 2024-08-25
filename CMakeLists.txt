cmake_minimum_required(VERSION 3.10)

# Project name and version
project(WebRTDB VERSION 1.0)

# Set C++11 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory for dependencies
include_directories(${PROJECT_SOURCE_DIR}/dependencies/include)

# Specify the executable
add_executable(WebRTDB 
    src/main.cpp
    src/rtdb/rtdb_wsserver.cpp
)
