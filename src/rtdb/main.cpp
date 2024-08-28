#include "rtdb_wsserver.h"
#include "rtdb_logger.h"
#include "rtdb_application.h"

#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    rtdb::AppConfig config = {
        .port=9002
    }; 

    rtdb::Application application(config);

    application.start();

    application.join();

    return 0;
}