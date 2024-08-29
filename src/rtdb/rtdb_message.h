#ifndef RTDB_MESSAGE_H
#define RTDB_MESSAGE_H

#include "rtdb_guid.h"

namespace rtdb {

class Message {
  private:
    GUID d_guid;
    long d_createdTimestamp;
    long d_updatedTimestamp;
    char *d_data;
    size_t d_dataSize;

  public:
    Message();
};

} // namespace rtdb

#endif