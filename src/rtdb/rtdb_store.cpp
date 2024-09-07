#include "rtdb_store.h"

namespace rtdb {

Store::DLLNode::DLLNode(Value& val) : message(std::make_unique(val)) {

}

Store::DLLNode::DLLNode() {}; // everything null

Store::Store(Ledger &ledger, std::string uid) : d_ledger(ledger), d_uid(uid) {
    // Set up initial head and tail sentinel nodes
    headNode = std::make_shared();
    tailNode = std::make_shared();
    headNode->prev = nullptr;
    headNode->next = tailNode;
    tailNode->prev = headNode;
    tailNode->next = nullptr;
}

Store::~Store() {
}

void Store::createMessage(Value& val) {
    std::shared_ptr<DLLNode> newNode = std::make_shared(val);
    {
        std::unique_lock acquireLock{d_lock};
        // insert newNode at the tail of the list
        tailNode->prev->next = newNode;
        newNode->next = tailNode;
        tailNode->prev = newNode;
        d_idToNode.insert({newNode->message->guid(), newNode});
    }
}

} // namespace rtdb