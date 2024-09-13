#ifndef RTDB_STORE_MANAGER_H
#define RTDB_STORE_MANAGER_H

#include "rtdb_store.h"

#include <memory>
#include <unordered_map>

namespace rtdb {

// Manages the creation and all store instances
class StoreManager {
  private:
    Ledger &d_ledger;
    std::unordered_map<std::string, std::unique_ptr<Store>> d_stores;

  public:
    StoreManager() = delete;
    StoreManager(Ledger &ledger);

  private:
    // Called when trying to access a store that does not yet exist
    // (the user should never explicitly have to create a store)
    std::unique_ptr<Store> &createStore(const std::string &id);

  public:
    std::unique_ptr<Store> &getStore(const std::string &id);
};

} // namespace rtdb

#endif