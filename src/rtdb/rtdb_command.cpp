#include "rtdb_command.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>

#include "rtdb_utils.h"

namespace rtdb {

CommandParseError::CommandParseError(std::string msg)
    : std::runtime_error(msg) {}

CommandToken::CommandToken(CommandTokenType type, const std::string &content)
    : type(type), content(content) {}

CommandToken::CommandToken(const Value &value)
    : type(e_TOK_VALUE), value(value) {}

Command::Command(const std::string &commandString)
    : d_commandString(commandString) {
    parse();
}

Command::~Command() {}

inline bool isAlphaOrUnderscore(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

using scit = std::string::const_iterator;

// A word is a contiguous segment of letters and underscores
// Advances the given iterator until the word is over or the end is reached
// outputs the content to the given stringstream, `ss`.
void advanceWord(scit &it, const scit &end, std::stringstream &ss) {
    while (isAlphaOrUnderscore(*it) && it != end) {
        ss << static_cast<char>(tolower(*it));
        it++;
    }
}

CommandTokenVector tokenize(std::string commandString) {
    std::vector<CommandToken> tokens;

    std::string::const_iterator it = commandString.begin();
    const scit &end = commandString.end();

    utils::advanceWhiteSpace(it, end);
    while (it != end) {
        std::stringstream ss;
        CommandTokenType tokType;

        char c = *it;

        // First try to interpret as a value.
        try {
            scit vit = it;
            Value value = Value::parse(vit, end);
            // if got here then it succeeded.
            it = vit;
            tokens.emplace_back(value);
            utils::advanceWhiteSpace(it, commandString.end());
            continue;
        } catch (const ValueParseError &e) {
        }

        // If we didn't properly parse as a value, then try special cases:
        // operation, equal sign, or identifier.

        if (isAlphaOrUnderscore(c)) {
            advanceWord(it, end, ss);
            if (commandOperationStrings.find(ss.str()) !=
                commandOperationStrings.end()) {
                tokType = e_TOK_OPERATION;
            } else {
                tokType = e_TOK_IDENTIFER;
            }
        } else if (c == '=') {
            ss << c;
            it++;
            tokType = e_TOK_EQUALS;
        } else {
            throw CommandParseError("Unexpected character while tokenizing: " +
                                    std::string{c});
        }

        tokens.emplace_back(tokType, ss.str());
        utils::advanceWhiteSpace(it, commandString.end());
    }

    return tokens;
}

void Command::parse() {
    CommandTokenVector tokens = tokenize(d_commandString);

    if (tokens.size() == 0) {
        throw CommandParseError("No tokens found while parsing command");
    }

    if (tokens[0].type != e_TOK_OPERATION) {
        throw CommandParseError("Unknown operation: \'" + tokens[0].content +
                                "\'");
    }

    std::string &operationName = tokens[0].content;

    auto operationPair = commandOperationStrings.find(operationName);
    // Tokenization should guarantee the content of a TOK_OPERATION is valid
    assert(operationPair != commandOperationStrings.end());

    d_operation = operationPair->second;

    CommandConfigParams &params = commandOperationConfig[d_operation];

    for (int i = 1; i < tokens.size(); i++) {
        // Expect identifer, then equals, then value
        if (tokens[i].type != e_TOK_IDENTIFER) {
            throw CommandParseError("Expected identifer token");
        }

        std::string &identifier = tokens[i].content;
        auto identifierPair = params.find(identifier);
        if (identifierPair == params.end()) {
            throw CommandParseError("Unrecognized parameter for \'" +
                                    tokens[0].content + "\': " + identifier);
        }

        i++;
        if (i >= tokens.size()) {
            throw CommandParseError("Expected equal sign after identifier");
        }

        if (tokens[i].type != e_TOK_EQUALS) {
            throw CommandParseError(
                "Unexpected token after identifer. Expected equal sign");
        }

        i++;
        if (i >= tokens.size()) {
            throw CommandParseError("Expected value after equal sign");
        }

        if (tokens[i].type != e_TOK_VALUE) {
            throw CommandParseError(
                "Unexpected token after identifier. Expected value");
        }

        Value &val = tokens[i].value;
        if ((val.type() & identifierPair->second) == 0) {
            throw CommandParseError("Expected different type for parameter \'" +
                                    identifier + "\'");
        }

        d_kv[identifier] = val;
    }
}

} // namespace rtdb
