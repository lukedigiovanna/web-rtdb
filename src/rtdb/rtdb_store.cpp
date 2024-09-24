#include "rtdb_store.h"

#include "rtdb_logger.h"
#include "rtdb_responseencoder.h"

namespace rtdb {

Store::DLLNode::DLLNode(const Value &val)
    : message(std::make_unique<Message>(val)) {}

Store::DLLNode::DLLNode() {}; // everything null

Store::Store(Ledger &ledger, const std::string &uid)
    : d_ledger(ledger), d_uid(uid) {
    // Set up initial head and tail sentinel nodes
    headNode = std::make_shared<DLLNode>();
    tailNode = std::make_shared<DLLNode>();
    headNode->prev = nullptr;
    headNode->next = tailNode;
    tailNode->prev = headNode;
    tailNode->next = nullptr;
}

Store::~Store() {}

void Store::createMessage(const Value &val) {
    std::shared_ptr<DLLNode> newNode = std::make_shared<DLLNode>(val);
    {
        std::unique_lock _{d_dataLock};
        // insert newNode at the tail of the list
        tailNode->prev->next = newNode;
        newNode->next = tailNode;
        tailNode->prev = newNode;
        d_idToNode.insert({newNode->message->guid(), newNode});
    }
    emitMessageUpdate(newNode->message);
}

void Store::updateMessage(const GUID &guid, const Value &val) {}

void Store::deleteMessage(const GUID &guid) {
    // Use map to quickly find the node associated with this message.
}

void Store::subscribe(WSServer::ConnectionSp conn) {
    std::unique_lock<std::mutex> _{d_subscriberLock};
    d_subscribers.insert(conn);
}

void Store::unsubscribe(WSServer::ConnectionSp conn) {
    std::unique_lock<std::mutex> _{d_subscriberLock};
    d_subscribers.erase(conn);
}

void Store::emitMessageUpdate(const std::unique_ptr<Message> &msg) {
    std::unique_lock _{d_subscriberLock};

    for (const auto &conn : d_subscribers) {
        conn->send(ResponseEncoder::encodeMessage(*msg));
    }
}

void Store::emitMessageDelete(const std::unique_ptr<Message> &msg) {}

} // namespace rtdb