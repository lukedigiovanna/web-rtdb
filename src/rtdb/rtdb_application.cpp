#include "rtdb_application.h"

#include "rtdb_logger.h"

namespace rtdb {


Application::Application(const AppConfig& config) : 
    d_server(config.port) {

}

void Application::start() {
    LOG_INFO << "Starting application";

    d_server.start();
}

}