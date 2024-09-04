#ifndef RTDB_COMMAND_H
#define RTDB_COMMAND_H

#include <array>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "rtdb_value.h"

namespace rtdb {

struct CommandParseError : public std::runtime_error {
    CommandParseError(std::string msg);
    CommandParseError() = delete; // Always require message
    ~CommandParseError() {}
};

// Parsing helpers

enum CommandTokenType {
    e_TOK_OPERATION,
    e_TOK_IDENTIFER,
    e_TOK_EQUALS,
    e_TOK_VALUE,
};

enum CommandOperation {
    e_SUBSCRIBE_ALL,
    e_SUBSCRIBE,
    e_PUSH,
    e_QUERY,
    e_DELETE,
    e_UPDATE
};

using CommandOperationStringMap = std::map<std::string, CommandOperation>;
static CommandOperationStringMap commandOperationStrings = {
    {"subscribe_all", e_SUBSCRIBE_ALL},
    {"subscribe", e_SUBSCRIBE},
    {"push", e_PUSH},
    {"query", e_QUERY},
    {"delete", e_DELETE},
    {"update", e_UPDATE},
};

using CommandConfigStore = std::map<CommandOperation, std::map<std::string, ValueType>>;
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
         {"ttl", e_INT},
         {"payload", e_JSON_OBJECT},
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
    std::string content;
    Value value; // Should only be populated if type is e_TOK_VALUE.

    CommandToken(CommandTokenType type, const std::string& content);
    CommandToken(const Value& value);
};

using CommandTokenVector = std::vector<CommandToken>;
using CommandKV = std::unordered_map<std::string, CommandKeyValue>;

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
