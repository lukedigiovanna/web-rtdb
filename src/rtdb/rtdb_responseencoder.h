#ifndef RTDB_RESPONSEENCODER_H
#define RTDB_RESPONSEENCODER_H

#include <string>

#include "rtdb_message.h"

namespace rtdb {

class ResponseEncoder {
  public:
    // static std::string encodeMessages(const std::vector<Message> )
    // Encodes a message in the proper response format
    static std::string encodeMessage(const Message& msg);
    // Encodes an error message in the proper response format
    static std::string encodeError(const std::string& errMsg);
};

}

#endif