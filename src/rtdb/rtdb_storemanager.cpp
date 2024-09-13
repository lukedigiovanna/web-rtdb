#include "rtdb_storemanager.h"

#include <assert.h>

namespace rtdb {

StoreManager::StoreManager(Ledger &ledger) : d_ledger(ledger) {}

std::unique_ptr<Store> &StoreManager::getStore(const std::string &id) {
    std::unique_lock<std::mutex> _{d_lock};

    auto f = d_stores.find(id);
    if (f == d_stores.end()) {
        d_stores.insert({id, std::make_unique<Store>(d_ledger, id)});
        return d_stores[id];
    } else {
        return f->second;
    }
}

void StoreManager::purgeSubscriptions(WSServer::ConnectionSp conn) {
    std::unique_lock<std::mutex> _{d_lock};
    // TODO: Make this more efficient
    // This could possibly be done by keeping track of who each connection is
    // subscribed to and only unsubscribe from those stores (likely to be small)
    // This can be inefficient if there are many stores, but this is not
    // currently a concern.
    for (const auto &storePair : d_stores) {
        storePair.second->unsubscribe(conn);
    }
}

} // namespace rtdb