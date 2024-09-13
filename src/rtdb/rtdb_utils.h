#ifndef RTDB_UTILS_H
#define RTDB_UTILS_H

#include <chrono>
#include <string>

namespace rtdb {
namespace utils {

inline bool isWhiteSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

inline void advanceWhiteSpace(std::string::const_iterator &it,
                              const std::string::const_iterator &end) {
    while (isWhiteSpace(*it) && it != end) {
        it++;
    }
}

inline long timeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

} // namespace utils
} // namespace rtdb

#endif