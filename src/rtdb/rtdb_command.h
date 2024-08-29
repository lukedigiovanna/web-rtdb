#ifndef RTDB_COMMAND_H
#define RTDB_COMMAND_H

#include <array>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace rtdb {

struct CommandParseError : public std::runtime_error {
    CommandParseError(std::string msg);
    CommandParseError() = delete; // Always require message
    ~CommandParseError() {}
};

// Parsing helpers

enum CommandTokenType {
    e_OPERATION,
    e_IDENTIFER,
    e_EQUALS,
    e_STRING_LITERAL,
    e_INT_LITERAL,
    e_FLOAT_LITERAL,
};

enum CommandKeyValueType {
    e_OPTIONAL = 1 << 0,
    e_FLOAT = 1 << 1,
    e_INT = 1 << 2,
    e_STRING = 1 << 3,
    e_JSON = 1 << 4,
};

enum CommandOperation {
    e_SUBSCRIBE_ALL,
    e_SUBSCRIBE,
    e_PUSH,
    e_QUERY,
    e_DELETE,
    e_UPDATE
};

typedef std::map<std::string, CommandOperation> CommandOperationStringMap;
static CommandOperationStringMap commandOperationStrings = {
    {"subscribe_all", e_SUBSCRIBE_ALL},
    {"subscribe", e_SUBSCRIBE},
    {"push", e_PUSH},
    {"query", e_QUERY},
    {"delete", e_DELETE},
    {"update", e_UPDATE},
};

typedef std::map<CommandOperation, std::map<std::string, int>>
    CommandConfigStore;
static CommandConfigStore commandOperationKws = {
    {e_SUBSCRIBE_ALL,
     {
         {"storeId", e_STRING},
     }},
    {e_SUBSCRIBE,
     {
         {"storeId", e_STRING},
         {"msgId", e_STRING},
     }},
    {e_PUSH,
     {
         {"storeId", e_STRING},
         {"ttl", e_OPTIONAL | e_FLOAT | e_INT},
         {"payload", e_JSON},
     }},
    {e_QUERY,
     {

     }},
    {e_DELETE,
     {

     }},
    {e_UPDATE,
     {

     }},
};

struct CommandKeyValue {
  private:
    float d_floatValue;
    long d_longValue;
    std::string d_strValue;

  public:
    CommandKeyValue();

    const float floatVal() const;
    const long longVal() const;
    const std::string stringVal() const;
};

struct CommandToken {
    CommandTokenType type;
    std::string value;
};

typedef std::vector<CommandToken> CommandTokenVector;
typedef std::unordered_map<std::string, CommandKeyValue> CommandKV;

class Command {
  private:
    const std::string &d_commandString;

    CommandOperation d_operation;
    CommandKV d_kv;

  public:
    Command(const std::string &commandString);
    ~Command();

    // Delete copy constructor
    Command(const Command &rhs) = delete;

  private:
    // Called internally by the constructor
    // Parses the commandString
    void parse();
};

} // namespace rtdb

#endif
