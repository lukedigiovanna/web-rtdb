#ifndef RTDB_STORE_H
#define RTDB_STORE_H

#include "rtdb_ledger.h"

#include <string>
#include <unordered_map>
#include <set>

#include <websocketpp/connection.hpp>

namespace rtdb {

// Stores
class Store {
private:
    // identifer
    std::string d_uid;
    std::unordered_map<std::string, void*> d_data;
    std::set<websocketpp::connection_hdl> d_subscribers;

    const Ledger& d_ledger;

public:
    Store() = delete;

    Store(const Ledger& ledger);
    ~Store();

    void publishCreateValue(void* val);
    void publishValue(std::string uid, void* val);

    static Ledger restoreFromLedger(const Ledger& ledger);
private:
    // Emits the value with the given uid to all subscribed clients.
    void emitValue(std::string uid);
};

}

#endif