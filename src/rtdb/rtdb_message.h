#ifndef RTDB_MESSAGE_H
#define RTDB_MESSAGE_H

#include "rtdb_guid.h"
#include "rtdb_value.h"

namespace rtdb {

class Message {
  private:
    GUID d_guid;
    long d_createdTimestamp;
    long d_updatedTimestamp;
    Value d_value;
    size_t d_dataSize;

  public:
    Message();
};

} // namespace rtdb

#endif