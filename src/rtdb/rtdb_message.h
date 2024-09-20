#ifndef RTDB_MESSAGE_H
#define RTDB_MESSAGE_H

#include "rtdb_guid.h"
#include "rtdb_utils.h"
#include "rtdb_value.h"

#include <string>

namespace rtdb {

class Message {
  private:
    std::string d_guid;
    // The system time this message was created.
    long d_createdTimestamp;
    // The system time of the last state update to this message
    long d_updatedTimestamp;
    // Amount of time since creation until this message should be purged.
    int d_ttl;
    // The actual value stored in this message.
    Value d_value;

    // Part of caching, so mark mutable to not affect const-ness
    mutable std::string d_cachedJson;
    mutable bool d_dirty;

  public:
    // Makes a copy of the given value
    Message(const Value &value);

    // Accessors
    const std::string &guid() const;
    long createdAt() const;
    long updatedAt() const;
    int ttl() const;
    bool dead() const;
    const Value &value() const;

    // Constructs a JSON representation of this message.
    std::string json() const;

    // Modifiers
    void updateValue(Value &value);
};

inline const std::string &Message::guid() const { return d_guid; }

inline long Message::createdAt() const { return d_createdTimestamp; }

inline long Message::updatedAt() const { return d_updatedTimestamp; }

inline int Message::ttl() const { return d_ttl; }

inline bool Message::dead() const {
    return d_ttl < 0 ? false
                     : utils::timeMillis() - d_createdTimestamp >= d_ttl;
}

inline const Value &Message::value() const { return d_value; }

} // namespace rtdb

#endif