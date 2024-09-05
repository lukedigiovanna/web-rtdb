#ifndef RTDB_VALUE_H
#define RTDB_VALUE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <stdexcept>

namespace rtdb {

enum ValueType {
    e_FLOAT,
    e_INT,
    e_STRING,
    e_JSON_OBJECT,
    e_JSON_ARRAY,
    e_NULL
};

struct ValueParseError : public std::runtime_error {
    ValueParseError(std::string msg);
    ValueParseError() = delete; // Always require message
    ~ValueParseError() {}
};

// Flexible type used to store a dynamic value. This is useful in
// storing messages within stores as well as in command key-value
// parsing.
class Value {
  public:
    using JsonObject = std::unordered_map<std::string, Value>;
    using JsonArray = std::vector<Value>;

  private:
    using VariantValue = std::variant<std::monostate, int, float, std::string,
                                      JsonArray, JsonObject>;

    ValueType d_type;
    VariantValue d_value;

  public:
    // Parse the string given starting at `it` and not going beyond
    // the iterator at `strEnd`. Stops when the value has been fully
    // parsed or throws a ValueParseError if the end is reached before
    // successfully processed.
    static Value parse(std::string::const_iterator &it,
                       const std::string::const_iterator &strEnd);

    // Initializes a null-valued value.
    Value();

    Value(int intVal);
    Value(float floatVal);
    Value(const std::string &stringVal);
    Value(const JsonObject &jsonObjVal);
    Value(const JsonArray &jsonArrayVal);

    // Accessors

    ValueType type() const;
    // Gets a string representation of whatever value is stored in this object.
    std::string str() const;

    int asInt() const;
    float asFloat() const;
    const std::string &asString() const;
    const JsonObject &asJsonObject() const;
    const JsonArray &asJsonArray() const;

    bool isNull() const;
};

inline ValueType Value::type() const { return d_type; }

inline int Value::asInt() const {
    if (d_type != e_INT) {
        throw std::bad_variant_access();
    }
    return std::get<int>(d_value);
}

inline float Value::asFloat() const {
    if (d_type != e_FLOAT) {
        throw std::bad_variant_access();
    }
    return std::get<float>(d_value);
}

inline const std::string &Value::asString() const {
    if (d_type != e_STRING) {
        throw std::bad_variant_access();
    }
    return std::get<std::string>(d_value);
}

inline const Value::JsonObject &Value::asJsonObject() const {
    if (d_type != e_JSON_OBJECT) {
        throw std::bad_variant_access();
    }
    return std::get<Value::JsonObject>(d_value);
}

inline const Value::JsonArray &Value::asJsonArray() const {
    if (d_type != e_JSON_ARRAY) {
        throw std::bad_variant_access();
    }
    return std::get<JsonArray>(d_value);
}

inline bool Value::isNull() const { return d_type == e_NULL; }

}; // namespace rtdb

#endif