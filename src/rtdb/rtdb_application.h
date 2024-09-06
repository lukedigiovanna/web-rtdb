#ifndef RTDB_APPLICATION
#define RTDB_APPLICATION

#include "rtdb_command.h"
#include "rtdb_ledger.h"
#include "rtdb_storemanager.h"
#include "rtdb_wsserver.h"

#include <thread>

namespace rtdb {

struct AppConfig {
    unsigned short port;
};

class Application {
  private:
    WSServer d_server;
    std::thread d_serverThread;

    Ledger d_ledger;
    StoreManager d_storeManager;

    bool d_running;

  public:
    Application(const AppConfig &config);

    // Should be executed by any thead in the command execution threadpool.
    void processCommand(WSServer::ConnectionSp conn, const Command &command);

    // Can be called by any thread to enqueue a command for execution.
    void enqueueCommand(WSServer::ConnectionSp conn, const Command& command);

    // Starts listening to connections
    void start();
    // Joins all the threads associated with this application.
    void join();

    bool running() const;
};

inline bool Application::running() const { return d_running; }

} // namespace rtdb

#endif