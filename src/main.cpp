#include "rtdb/rtdb_wsserver.h"

#include <iostream>

int main(int argc, char* argv[]) {
    rtdb::WSServerConfig config = {
        .port=9002
    };    
    rtdb::WSServer server(config);

    server.run();

    return 0;
}