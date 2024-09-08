#include "rtdb_message.h"

#include "rtdb_utils.h"

namespace rtdb {

Message::Message(const Value& value) :
    d_value(value), 
    d_createdTimestamp(utils::timeMillis()),
    d_updatedTimestamp(d_createdTimestamp) {
}

}