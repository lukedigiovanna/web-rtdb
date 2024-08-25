cmake_minimum_required(VERSION 3.10)
project(WebRTDB)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the ASIO_STANDALONE and _WEBSOCKETPP_CPP11_THREAD macros
add_definitions(-DASIO_STANDALONE)
add_definitions(-D_WEBSOCKETPP_CPP11_THREAD_)

# Include directories (adjust these paths as needed)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)

# Add your source files
add_executable(WebRTDB 
    src/rtdb/main.cpp 
    src/rtdb/rtdb_wsserver.cpp
)

# Link against Ws2_32.lib
if(WIN32)
    target_link_libraries(WebRTDB PRIVATE ws2_32)
endif()
