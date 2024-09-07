#ifndef RTDB_STORE_H
#define RTDB_STORE_H

#include "rtdb_ledger.h"
#include "rtdb_message.h"
#include "rtdb_value.h"
#include "rtdb_wsserver.h"

#include <mutex>
#include <set>
#include <string>
#include <unordered_map>

#include <websocketpp/connection.hpp>

namespace rtdb {


// A store is specialized data structure for storing messages in a time-ordered
// series. It enables efficient lookups of alive messages and pruning of dead
// messages. It also enables efficient lookups and modifications to messages
// by id. 
class Store {
  private:
    struct DLLNode {
      DLLNode* prev;
      DLLNode* next;
      Message message;

      DLLNode(Value& val);
      DLLNode(const DLLNode&) = delete;
    };
  private:
    // identifer
    std::string d_uid;

    DLLNode* headNode;
    DLLNode* tailNode;

    std::unordered_map<std::string, DLLNode*> d_idToNode;
    std::set<WSServer::ConnectionSp> d_subscribers;
    std::mutex d_lock;

    const Ledger &d_ledger;

  public:
    Store() = delete;

    Store(const Ledger &ledger, std::string& uid);
    ~Store();

    void publishCreateValue(Value& val);
    void publishValue(std::string uid, Value& val);

    void subscribe(WSServer::ConnectionSp conn);

    static Ledger restoreFromLedger(const Ledger &ledger);

  private:
    // Emits the value with the given uid to all subscribed clients.
    void emitValue(std::string uid);
};

} // namespace rtdb

#endif