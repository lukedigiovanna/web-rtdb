#include "rtdb_guid.h"

#include <stdlib.h>

namespace rtdb {

GUID::GUID() {
    for (int i = 0; i < GUID_LENGTH; i++) {
        d_val[i] = GUID_ALPHABET[rand() % GUID_ALPHABET_LENGTH];
    }
}

GUID::GUID(const GUID& guid) {
    for (int i = 0; i < GUID_LENGTH; i++) {
        d_val[i] = guid.d_val[i];
    }
}

GUID::~GUID() {
    
}

}