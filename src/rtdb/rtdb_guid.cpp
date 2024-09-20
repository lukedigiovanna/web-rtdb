#include "rtdb_guid.h"

#include "rtdb_logger.h"

#include <stdlib.h>

namespace rtdb {

GUID::GUID() {
    for (int i = 0; i < GUID_LENGTH; i++) {
        d_val[i] = GUID_ALPHABET[rand() % GUID_ALPHABET_LENGTH];
    }
    // Null terminate.
    d_val[GUID_LENGTH] = '\0';
}

GUID::GUID(const GUID &guid) {
    for (int i = 0; i < GUID_LENGTH; i++) {
        d_val[i] = guid.d_val[i];
    }
}

std::string GUID::generateStr() {
    std::string str(' ', GUID_LENGTH);
    for (int i = 0; i < GUID_LENGTH; i++) {
        str[i] = GUID_ALPHABET[rand() % GUID_ALPHABET_LENGTH];
    }
    LOG_WARNING << "generated guid: \"" << str << "\"";
    return str;
}

} // namespace rtdb