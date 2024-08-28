#ifndef RTDB_COMMAND_H
#define RTDB_COMMAND_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace rtdb {

struct CommandParseError : public std::runtime_error {
    CommandParseError(std::string msg);
    CommandParseError() = delete; // Always require message
    ~CommandParseError() {}
};

union CommandKVType {
    long d_longValue;
    std::string d_strValue;
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

static std::vector<std::string> commandOperationKws = {
    "subscribe_all",
    "subscribe",
    "push",
    "query",
    "delete",
    "update"
};;

struct CommandToken {
    CommandTokenType type;
    std::string value;
};

typedef std::vector<CommandToken> CommandTokenVector;
// typedef std::unordered_map<std::string, CommandKVType> CommandKV;

class Command {
private:
    const std::string& d_commandString;

    // CommandKV d_kv;

public:
    Command(const std::string& commandString);
    ~Command();

    // Delete copy constructor
    Command(const Command& rhs) = delete;

private:
    // Called internally by the constructor
    // Parses the commandString 
    void parse();
};

}

#endif
