#ifndef RTDB_STORE_MANAGER_H
#define RTDB_STORE_MANAGER_H

#include "rtdb_store.h"

#include <memory>
#include <unordered_map>

namespace rtdb {

// Manages the creation and all store instances
class StoreManager {
public:
    typedef std::unique_ptr<Store> StorePtr;
private:
    Ledger d_ledger;
    std::unordered_map<std::string, StorePtr> d_stores;
public:
    StoreManager();

    StorePtr createStore(std::string id);
    StorePtr getStore(std::string id);
};

}

#endif