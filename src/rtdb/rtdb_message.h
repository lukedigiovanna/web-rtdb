#ifndef RTDB_MESSAGE_H
#define RTDB_MESSAGE_H

#include "rtdb_guid.h"

namespace rtdb {

class Message {
private:
    GUID guid;
public:
    Message();
};

}

#endif