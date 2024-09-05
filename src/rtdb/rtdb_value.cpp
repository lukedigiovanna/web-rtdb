#include "rtdb_value.h"

#include "rtdb_logger.h"

#include <assert.h>
#include <sstream>

namespace rtdb {

ValueParseError::ValueParseError(std::string msg) : std::runtime_error(msg) {}

Value::Value() : d_type(e_NULL) {}

Value::Value(int intVal) : d_type(e_INT), d_value(intVal) {}

Value::Value(float floatVal) : d_type(e_FLOAT), d_value(floatVal) {}

Value::Value(const std::string &stringVal)
    : d_type(e_STRING), d_value(stringVal) {}

Value::Value(const Value::JsonObject &jsonObjVal)
    : d_type(e_JSON_OBJECT), d_value(jsonObjVal) {}

Value::Value(const Value::JsonArray &jsonArrayVal)
    : d_type(e_JSON_ARRAY), d_value(jsonArrayVal) {}

std::string Value::str() const {
    switch (d_type) {
    case e_FLOAT:
        return std::to_string(asFloat());
    case e_INT:
        return std::to_string(asInt());
    case e_STRING:
        return asString();
    case e_NULL:
        return "null";
    case e_JSON_OBJECT:
        // reconstruct compact JSON string
        return "";
    case e_JSON_ARRAY:
        // reconstruct compact JSON array
        return "";
    default:
        return "";
    }
}

// Helper functions

using scit = std::string::const_iterator;

std::unordered_map<char, char> escChars = {
    {'\\', '\\'}, {'\'', '\''}, {'\"', '\"'}, {'t', '\t'}, {'n', '\n'},
};

// A string is a segment of text between two non-escaped quotations.
// Advances the given iterator until the end of the string (1 char after the
// final quote). Will throw an error if it reaches the end of the string
// without encountering a closing quotation.
void parseString(scit &it, const scit &end, std::stringstream &ss) {
    LOG_WARNING << "Starting parseString";
    assert(*it == '\"');
    it++;
    while (*it != '\"') {
        if (it == end) {
            throw ValueParseError(
                "Error parsing string: missing end quotation in " + ss.str());
        }
        char c = *it;
        if (c == '\\') {
            if (it + 1 == end) {
                // If one past a backslash is the end then we are definitely
                // missing a quotation.
                throw ValueParseError(
                    "Error parsing string: missing end quotation in " +
                    ss.str());
            }
            char nc = *(it + 1);
            auto f = escChars.find(nc);
            if (f == escChars.end()) {
                throw ValueParseError(
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
    LOG_WARNING << "Finished parseString";
}

ValueType parseNumeric(scit &it, const scit &strEnd, std::stringstream &ss) {
    bool foundDecimalPoint = false;
    while (it != strEnd) {
        if (isdigit(*it)) {
            ss << *it;
        } else if (*it == '.' && !foundDecimalPoint) {
            ss << *it;
            foundDecimalPoint = true;
        } else {
            break;
        }
        it++;
    }
    return foundDecimalPoint ? e_FLOAT : e_INT;
}

// Simply checks the iterator matches "null" and iterates the given
// iterator past null. If this is not a match, then an error is thrown.
void parseNull(scit &it, const scit &strEnd) {
    // Check if there is an exact match to "null".
    const std::string null = "null";
    for (const auto c : null) {
        if (it == strEnd || *it != c) {
            throw ValueParseError("Expected 'null' value.");
        }
        it++;
    }
}

void parseJsonObject(scit &it, const scit &strEnd) {}

void parseJsonArray(scit &it, const scit &strEnd) {}

Value Value::parse(scit &it, const scit &strEnd) {
    if (*it == '\"') {
        std::stringstream ss;
        parseString(it, strEnd, ss);
        return Value(ss.str());
    } else if (isdigit(*it)) {
        std::stringstream ss;
        ValueType type = parseNumeric(it, strEnd, ss);
        if (type == e_INT) {
            int v;
            ss >> v;
            return Value(v);
        } else { // type == e_FLOAT
            float f;
            ss >> f;
            return Value(f);
        }
    } else if (*it == '{') {
        // Parse JSON object
    } else if (*it == '[') {
        // Parse JSON array
    } else if (*it == 'n') {
        parseNull(it, strEnd);
        return Value();
    } else {
        throw ValueParseError("Unexpected token while parsing value: " +
                              std::string(1, *it));
    }
}

} // namespace rtdb