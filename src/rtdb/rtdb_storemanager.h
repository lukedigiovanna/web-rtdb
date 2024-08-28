#ifndef RTDB_STORE_MANAGER_H
#define RTDB_STORE_MANAGER_H

#include "rtdb_store.h"

#include <memory>
#include <unordered_map>

namespace rtdb {

// 
class StoreManager {
public:
    typedef std::unique_ptr<Store> StoreUPtr;
private:
    Ledger d_ledger;
    std::unordered_map<std::string, StoreUPtr> d_stores;
public:
    StoreManager();

    void getStore(std::string id);
};

}

#endif