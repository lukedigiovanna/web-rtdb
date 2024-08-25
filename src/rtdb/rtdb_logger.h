#ifndef RTDB_LOGGER_H
#define RTDB_LOGGER_H

#include <string>

class Logger {
public:
    static void debug(const std::string& msg);
};

#endif