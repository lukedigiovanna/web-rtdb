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
// by id. All operations on a store are thread safe.
class Store {
  private:
    struct DLLNode {
      std::shared_ptr<DLLNode> prev;
      std::shared_ptr<DLLNode> next;
      std::unique_ptr<Message> message;

      DLLNode();
      DLLNode(Value& val);
      
      DLLNode(const DLLNode&) = delete;
    };
  private:
    // identifer
    std::string d_uid;

    std::shared_ptr<DLLNode> headNode;
    std::shared_ptr<DLLNode> tailNode;

    std::unordered_map<GUID, std::shared_ptr<DLLNode>> d_idToNode;
    std::set<WSServer::ConnectionSp> d_subscribers;
    // Used for locking when accessing/modifying data.
    std::mutex d_dataLock;
    // Used for locking when accessing/modifying set of subscribers.
    std::mutex d_subscriberLock;

    Ledger &d_ledger;
  public:
    Store() = delete;

    Store(Ledger &ledger, std::string& uid);
    ~Store();

    void createMessage(Value& val);
    void updateMessage(const GUID& guid, Value& val);
    void deleteMessage(const GUID& guid);

    void cleanDeadMessages();

    void subscribe(WSServer::ConnectionSp conn);

    static Ledger restoreFromLedger(const Ledger &ledger);

  private:
    // Emits the value with the given uid to all subscribed clients.
    void emitMessage(std::weak_ptr<Message> msg);
};

} // namespace rtdb

#endif