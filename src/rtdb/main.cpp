#include "rtdb_wsserver.h"

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Starting program" << std::endl;
    std::cout.flush();

    rtdb::WSServerConfig config = {
        .port=9002
    };    
    rtdb::WSServer server(config);

    server.run();

    return 0;
}