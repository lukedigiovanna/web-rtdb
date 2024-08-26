#include "rtdb_application.h"

#include "rtdb_logger.h"

namespace rtdb {


Application::Application(const AppConfig& config) : 
    d_server(config.port),
    d_running(false) {

}

void Application::start() {
    LOG_INFO << "Starting application";
    d_running = true;
    d_serverThread = std::thread([&]() {
        LOG_INFO << "Starting server thread";
        d_server.start();
    });
}

void Application::join() {
    assert(d_running);

    d_serverThread.join();

    d_running = false;
}

}