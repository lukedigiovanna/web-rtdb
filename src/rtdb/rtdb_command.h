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

using CommandConfigParams = std::map<std::string, ValueType>;
using CommandConfigStore = std::map<CommandOperation, CommandConfigParams>;
static CommandConfigStore commandOperationConfig = {
    {e_SUBSCRIBE_ALL,
     {
         {"storeid", e_STRING},
     }},
    {e_SUBSCRIBE,
     {
         {"storeid", e_STRING},
         {"msgid", e_STRING},
     }},
    {e_PUSH,
     {
         {"storeid", e_STRING},
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

struct CommandToken {
    CommandTokenType type;
    std::string content;
    Value value; // Should only be populated if type is e_TOK_VALUE.

    CommandToken(CommandTokenType type, const std::string &content);
    CommandToken(const Value &value);
};

using CommandTokenVector = std::vector<CommandToken>;
using CommandParams = std::unordered_map<std::string, Value>;

class Command {
  private:
    const std::string &d_commandString;

    CommandOperation d_operation;
    CommandParams d_kv;

  public:
    Command(const std::string &commandString);
    ~Command();

    // Delete copy constructor
    Command(const Command &rhs) = delete;

  private:
    // Called internally by the constructor
    // Parses the commandString
    void parse();
  
  public:
    // Accessors

    CommandOperation operation() const;
    const Value& getParameter(const std::string& parameterName) const;
    const std::string& str() const;
};

inline CommandOperation Command::operation() const {
    return d_operation;
}

inline const Value& Command::getParameter(const std::string& parameterName) const {
    auto f = d_kv.find(parameterName);
    if (f == d_kv.end()) {
        throw std::runtime_error("Parameter \'" + parameterName + "\' does not exist on this command.");
    }
    return f->second;
} 

inline const std::string& Command::str() const {
    return d_commandString;
}

} // namespace rtdb

#endif
