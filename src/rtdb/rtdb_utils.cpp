#include "rtdb_utils.h"

#define UID_ALPHABET "abcdefghijklmnopqrstuvwxyz0123456789"
#define UID_SIZE 16

namespace rtdb {

std::string Utils::genUID() {
    std::string uid(UID_SIZE, '\0');
    for (int i = 0; i < UID_SIZE; i++) {
        uid[i] = UID_ALPHABET[rand() % sizeof(UID_ALPHABET)];
    }
    return uid;
}

}