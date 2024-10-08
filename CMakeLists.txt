cmake_minimum_required(VERSION 3.10)
project(WebRTDB)

# Enable C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories (adjust these paths as needed)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)

# Add your source files
add_executable(WebRTDB 
    src/rtdb/main.cpp 
    src/rtdb/rtdb_application.cpp
    src/rtdb/rtdb_command.cpp
    src/rtdb/rtdb_guid.cpp
    src/rtdb/rtdb_logger.cpp
    src/rtdb/rtdb_ledger.cpp
    src/rtdb/rtdb_message.cpp
    src/rtdb/rtdb_responseencoder.cpp
    src/rtdb/rtdb_store.cpp
    src/rtdb/rtdb_storemanager.cpp
    src/rtdb/rtdb_value.cpp
    src/rtdb/rtdb_wsserver.cpp
)

# Link against Ws2_32.lib
if(WIN32)
    target_link_libraries(WebRTDB PRIVATE ws2_32 Mswsock)
endif()
