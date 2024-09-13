#include "rtdb_storemanager.h"

#include <assert.h>

namespace rtdb {

StoreManager::StoreManager(Ledger &ledger) : d_ledger(ledger) {}

std::unique_ptr<Store> &StoreManager::createStore(const std::string &id) {
    // Should never try to create a store that already exists.
    assert(d_stores.find(id) == d_stores.end());

    d_stores.insert({id, std::make_unique<Store>(d_ledger, id)});

    return d_stores[id];
}

std::unique_ptr<Store> &StoreManager::getStore(const std::string &id) {
    auto f = d_stores.find(id);
    if (f == d_stores.end()) {
        return createStore(id);
    } else {
        return f->second;
    }
}

} // namespace rtdb