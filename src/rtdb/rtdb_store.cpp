#include "rtdb_store.h"

#include "rtdb_utils.h"

namespace rtdb {

Store::Store(const Ledger& ledger) : d_ledger(ledger) {
    d_uid = Utils::genUID();
}

Store::~Store() {

}

}