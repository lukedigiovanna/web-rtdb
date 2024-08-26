#ifndef RTDB_APPLICATION
#define RTDB_APPLICATION

#include "rtdb_wsserver.h"

namespace rtdb {

struct AppConfig {
    unsigned short port;
};

class Application {
private:
    WSServer d_server;

public:
    Application(const AppConfig& config);

    void start();
};

}

#endif