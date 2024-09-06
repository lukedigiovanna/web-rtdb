#ifndef RTDB_STORE_MANAGER_H
#define RTDB_STORE_MANAGER_H

#include "rtdb_store.h"

#include <memory>
#include <unordered_map>

namespace rtdb {

// Manages the creation and all store instances
class StoreManager {
  public:
    using StorePtr = std::unique_ptr<Store>;

  private:
    Ledger d_ledger;
    std::unordered_map<std::string, StorePtr> d_stores;

  public:
    StoreManager() = delete;
    StoreManager(Ledger& ledger);

  private:
    // Called when trying to access a store that does not yet exist
    // (the user should never explicitly have to create a store)
    StorePtr createStore(std::string id);

  public:
    StorePtr getStore(std::string id);
};

} // namespace rtdb

#endif