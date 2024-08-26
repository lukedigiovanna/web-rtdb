#include "rtdb_logger.h"

#include <cstring>

namespace rtdb {

Logger::Logger(Level level, const char* file, int line) : logLevel(level) {
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // Format time
    stream << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setfill('0') << std::setw(1) << ms.count() << " ";

    // Log level as string
    switch (level) {
        case e_INFO: stream << "\033[32m" << "[INFO] "; break;
        case e_WARNING: stream << "\033[33m" << "[WARNING] "; break;
        case e_ERROR: stream << "\033[31m" << "[ERROR] "; break;
    }

    // Get the file name starting from "src/"
    const char* src_position = std::strstr(file, "src/");
    if (src_position) {
        stream << src_position << ":" << line << " ";
    } else {
        // If "src/" is not found, log the full file path
        stream << file << ":" << line << " ";
    }
}

Logger::~Logger() {
    stream << "\033[37m" << std::endl;
    std::cerr << stream.str();
}

}