#ifndef RTDB_APPLICATION
#define RTDB_APPLICATION

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

    StoreManager d_storeManager;
    Ledger d_ledger;

    bool d_running;
public:
    Application(const AppConfig& config);

    void start();

    void join();

    bool running() const;
};

inline bool Application::running() const {
    return d_running;
}

}

#endif