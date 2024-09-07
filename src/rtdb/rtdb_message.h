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

  public:
    Message(Value& value);

    // Accessors
    const GUID& guid() const;
    const long createdAt() const;
    const long updatedAt() const;
    const Value& value() const;

    // Modifiers
    void updateValue(Value& value) const;
};

inline const GUID& Message::guid() const {
  return d_guid;
}

inline const long Message::createdAt() const {
  return d_createdTimestamp;
}

inline const long Message::updatedAt() const {
  return d_updatedTimestamp;
}

inline const Value& Message::value() const {
  return d_value;
}

} // namespace rtdb

#endif