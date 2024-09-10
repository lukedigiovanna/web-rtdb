#include "rtdb_message.h"

#include "rtdb_utils.h"
#include <sstream>

namespace rtdb {

Message::Message(const Value& value) :
    d_value(value), 
    d_createdTimestamp(utils::timeMillis()),
    d_updatedTimestamp(d_createdTimestamp),
    d_dirty(true) {
}

std::string Message::json() {
    if (d_dirty) {
        std::stringstream ss;
        ss << "{";
        ss << "\"guid\":\"" << guid() << "\",";
        ss << "\"created\":" << createdAt() << ",";
        ss << "\"updated\":" << updatedAt() << ",";
        ss << "\"value\":" << value().str();
        ss << "}";
        d_cachedJson = ss.str();
        d_dirty = false;
    }
    return d_cachedJson;
}

void Message::updateValue(Value& value) {
    d_dirty = true;
    d_updatedTimestamp = utils::timeMillis();
    d_value = value;
}

}