#include "rtdb_store.h"

namespace rtdb {

Store::Store(const Ledger &ledger, std::string uid) : d_ledger(ledger), d_uid(uid) {

}

Store::~Store() {}

} // namespace rtdb