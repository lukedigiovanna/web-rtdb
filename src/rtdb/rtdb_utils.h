#ifndef RTDB_UTILS_H
#define RTDB_UTILS_H

#include <string>

namespace rtdb {
namespace utils {

inline bool isWhiteSpace(char c) { 
    return c == ' ' || c == '\t' || c == '\n';
}

inline void advanceWhiteSpace(std::string::const_iterator &it, const std::string::const_iterator &end) {
    while (isWhiteSpace(*it) && it != end) {
        it++;
    }
}
   
}
}

#endif