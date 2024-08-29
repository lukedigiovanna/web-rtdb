#include "rtdb_command.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include <assert.h>
#include <iostream>

namespace rtdb {

CommandParseError::CommandParseError(std::string msg)
    : std::runtime_error(msg) {}

Command::Command(const std::string &commandString)
    : d_commandString(commandString) {
    parse();
}

Command::~Command() {}

inline bool isAlphaOrUnderscore(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

inline bool isWhiteSpace(char c) { return c == ' ' || c == '\t'; }

inline char tolower(char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

std::unordered_map<char, char> escChars = {
    {'\\', '\\'}, {'\'', '\''}, {'\"', '\"'}, {'t', '\t'}, {'n', '\n'},
};

using scit = std::string::const_iterator;

void advanceWhiteSpace(scit &it, const scit &end) {
    while (isWhiteSpace(*it) && it != end) {
        it++;
    }
}

// A word is a contiguous segment of letters and underscores
// Advances the given iterator until the word is over or the end is reached
// outputs the content to the given stringstream, `ss`.
void advanceWord(scit &it, const scit &end, std::stringstream &ss) {
    while (isAlphaOrUnderscore(*it) && it != end) {
        ss << tolower(*it);
        it++;
    }
}

// A string is a segment of text between two non-escaped quotations.
// Advances the given iterator until the end of the string (1 char after the
// final quote). Will throw an error if it reaches the end of the string
// without encountering a closing quotation.
void advanceString(scit &it, const scit &end, std::stringstream &ss) {
    assert(("Advancing string must start on a quotation", *it == '\"'));
    it++;
    while (*it != '\"') {
        if (it == end) {
            throw CommandParseError(
                "Error parsing string: missing end quotation in " + ss.str());
        }
        char c = *it;
        if (c == '\\') {
            if (it + 1 == end) {
                // If one past a backslash is the end then we are definitely
                // missing a quotation.
                throw CommandParseError(
                    "Error parsing string: missing end quotation in " +
                    ss.str());
            }
            char nc = *(it + 1);
            auto f = escChars.find(nc);
            if (f == escChars.end()) {
                throw CommandParseError(
                    "Error parsing string: invalid escape sequence \\" +
                    std::string{nc});
            }
            ss << f->second;
            it++;
        } else {
            ss << c;
        }
        it++;
    }
    it++; // Go one past the quotation
}

CommandTokenVector tokenize(std::string commandString) {
    std::vector<CommandToken> tokens;

    std::string::const_iterator it = commandString.begin();
    const scit &end = commandString.end();
    advanceWhiteSpace(it, end);
    while (it != end) {
        std::stringstream ss;
        char c = *it;
        CommandTokenType tokType;
        if (c == '\"') {
            // parse String
            advanceString(it, end, ss);
            tokType = e_STRING_LITERAL;
        } else if (c == '{') {
            // parse JSON
        } else if (isAlphaOrUnderscore(c)) {
            advanceWord(it, end, ss);
            if (std::find(commandOperationKws.begin(),
                          commandOperationKws.end(),
                          ss.str()) != commandOperationKws.end()) {
                tokType = e_OPERATION;
            } else {
                tokType = e_IDENTIFER;
            }
        } else if (c == '=') {
            ss << c;
            it++;
            tokType = e_EQUALS;
        } else {
            throw CommandParseError("Unexpected character while tokenizing: " +
                                    std::string{c});
        }

        tokens.push_back({tokType, ss.str()});

        advanceWhiteSpace(it, commandString.end());
    }

    return tokens;
}

void Command::parse() {
    CommandTokenVector tokens = tokenize(d_commandString);

    if (tokens.size() == 0) {
        throw CommandParseError("No tokens found while parsing command");
    }

    if (tokens[0].type != e_OPERATION) {
        throw CommandParseError("First token in command must be an operation");
    }

    for (const auto &tok : tokens) {
        std::cout << tok.type << ": " << tok.value << std::endl;
    }
}

} // namespace rtdb
