#include "rtdb_message.h"

#include "rtdb_utils.h"
#include "rtdb_logger.h"
#include <sstream>

namespace rtdb {

Message::Message(const Value &value)
    : d_guid(GUID::generateStr()), d_value(value), d_createdTimestamp(utils::timeMillis()),
      d_updatedTimestamp(d_createdTimestamp), d_dirty(true) {}

std::string Message::json() const {
    if (d_dirty) {
        std::stringstream ss;
        ss << "{";
        ss << "\"guid\":\"" << d_guid << "\",";
        ss << "\"created\":" << d_createdTimestamp << ",";
        ss << "\"updated\":" << d_updatedTimestamp << ",";
        ss << "\"ttl\":" << d_ttl << ",";
        ss << "\"value\":" << d_value.str();
        ss << "}";
        d_cachedJson = ss.str();
        d_dirty = false;
    }
    return d_cachedJson;
}

void Message::updateValue(Value &value) {
    d_dirty = true;
    d_updatedTimestamp = utils::timeMillis();
    d_value = value;
}

} // namespace rtdb