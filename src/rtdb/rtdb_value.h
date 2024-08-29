#ifndef RTDB_VALUE_H
#define RTDB_VALUE_H

#include <string>

namespace rtdb {

enum ValueType {
    e_FLOAT,
    e_INT,
    e_STRING,
    e_JSON_OBJECT,
    e_JSON_ARRAY,
    e_NULL
};

class Value {
  private:
  public:
    static Value parse(std::string::const_iterator it,
                       std::string::const_iterator strEnd);
};

}; // namespace rtdb

#endif