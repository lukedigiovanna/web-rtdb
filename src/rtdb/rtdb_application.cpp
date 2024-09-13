#include "rtdb_application.h"

#include "rtdb_command.h"
#include "rtdb_logger.h"

namespace rtdb {

Application::Application(const AppConfig &config)
    : d_server(config.port), d_running(false), d_storeManager(d_ledger) {
    // Runs on the websocket server dedicated thread (d_serverThread)
    d_server.setOnMessageCallback(
        [&](WSServer::ConnectionSp conn, const Command &command) {
            enqueueCommand(conn, command);
        });

    d_server.setOnCloseCallback([&](WSServer::ConnectionSp conn) {
        d_storeManager.purgeSubscriptions(conn);
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

void Application::processCommand(WSServer::ConnectionSp conn,
                                 const Command &command) {
    LOG_INFO << "Application receieved command: " << command.str()
             << " (opcode: " << command.operation() << ")";
    switch (command.operation()) {
    case CommandOperation::e_PUSH: {
        // Push message to storeid
        const std::string &storeId = command.getParameter("storeid").asString();
        LOG_INFO << "Pushing to " << storeId;
        const Value &value = command.getParameter("value");
        std::unique_ptr<Store> &store = d_storeManager.getStore(storeId);
        store->createMessage(value);
    } break;
    case CommandOperation::e_QUERY:
        break;
    case CommandOperation::e_UPDATE:
        break;
    case CommandOperation::e_DELETE:
        break;
    case CommandOperation::e_SUBSCRIBE:
        break;
    case CommandOperation::e_SUBSCRIBE_ALL: {
        const std::string &storeId = command.getParameter("storeid").asString();
        LOG_INFO << "Subscribing to " << storeId;
        std::unique_ptr<Store> &store = d_storeManager.getStore(storeId);
        store->subscribe(conn);
    } break;
    }
}

void Application::enqueueCommand(WSServer::ConnectionSp conn,
                                 const Command &command) {
    // d_commandExecutionThreadpool();
    processCommand(conn, command);
}

} // namespace rtdb