#include "rtdb_responseencoder.h"

#include <sstream>

namespace rtdb {

std::string ResponseEncoder::encodeMessage(const Message &msg) {
    std::stringstream ss;
    ss << "{";
    ss << "\"type\":\"message_value\",";
    ss << "\"message\":" << msg.json();
    ss << "}";
    return ss.str();
}

std::string ResponseEncoder::encodeError(const std::string &errMsg) {
    std::stringstream ss;
    ss << "{";
    ss << "\"type\":\"error\",";
    ss << "\"message\":\"" << errMsg << "\"";
    ss << "}";
    return ss.str();
}

} // namespace rtdb
