#include "rtdb_application.h"

#include "rtdb_command.h"
#include "rtdb_logger.h"

namespace rtdb {

Application::Application(const AppConfig &config)
    : d_server(config.port), d_running(false) { 
    // Runs on the websocket server dedicated thread (d_serverThread)
    d_server.setOnMessageCallback(
        [&](WSServer::ConnectionSp conn, const Command &command) {
            enqueueCommand(conn, command);
        });
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

void Application::processCommand(WSServer::ConnectionSp conn, const Command& command) {
    LOG_INFO << "Application receieved command: " << command.str();
    switch (command.operation()) {
        case CommandOperation::e_PUSH:
            break;
        case CommandOperation::e_QUERY:
            break;
        case CommandOperation::e_UPDATE:
            break;
        case CommandOperation::e_DELETE:
            break;
        case CommandOperation::e_SUBSCRIBE:
            break;
        case CommandOperation::e_SUBSCRIBE_ALL:
            break;
    }
}

void Application::enqueueCommand(WSServer::ConnectionSp conn, const Command& command) {
    // d_commandExecutionThreadpool();
    processCommand(conn, command);
}

} // namespace rtdb