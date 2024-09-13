#ifndef RTDB_GUID_H
#define RTDB_GUID_H

#include <functional>
#include <ostream>

#define GUID_LENGTH 16
#define GUID_ALPHABET "abcdefghijklmnopqrstuvwxyz012456789"
#define GUID_ALPHABET_LENGTH sizeof(GUID_ALPHABET)

namespace rtdb {

// Analysis on the chances of collisions:
//
// This is an application of the "birthday problem". That is, given
// some number of instances of elements from a uniform distribution,
// what are the chances that any two of those are the same?
//
// For the general case of random strings, let's assume the following variables.
// Let N be the alphabet length, M be the length of the string, and Q be the
// number of instances we have.
//
// To determine the odds that at least 2 of the Q are the same, it is simpler
// to compute the odds that they are all different. This is done as
//                [ (N^M)! / (N^M - Q)! ] / [(N^M)^Q]
// We expect a collision once this value reaches 0.5.
//
// Suppose we generated IDs with 4 letters and length 4, we would expect a
// collision after just 20 generations.
//
// With the parameters used in this generator, there are over 10^24 possible
// GUIDs. In any practical application we will never expect a collision though
// it is technically possible.

class GUID {
  private:
    char d_val[GUID_LENGTH + 1];

  public:
    GUID();
    GUID(const GUID &guid);

    ~GUID() = default;

    const char *c_str() const;

    bool operator==(const GUID &rhs) const;

    friend struct std::hash<GUID>;
    friend std::ostream &operator<<(std::ostream &os, GUID const &guid);
};

inline const char *GUID::c_str() const { return d_val; }

inline bool GUID::operator==(const GUID &rhs) const {
    for (int i = 0; i < GUID_LENGTH; i++) {
        if (d_val[i] != rhs.d_val[i]) {
            return false;
        }
    }
    return true;
}

inline std::ostream &operator<<(std::ostream &os, rtdb::GUID const &guid) {
    os << guid.c_str();
    return os;
}

} // namespace rtdb

namespace std {

/// Implements the DJBX33A hash algorithm for simple strings.
template <> struct hash<rtdb::GUID> {
    size_t operator()(const rtdb::GUID &guid) const {
        size_t h = 5381;
        for (int i = 0; i < GUID_LENGTH; i++) {
            h = (h * 33) + guid.d_val[i];
        }
        return h;
    }
};

} // namespace std

#endif