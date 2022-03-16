#ifndef HASH_H
#define HASH_H

#include "string"
#include "vector"
#include "types.h"

using namespace std;
namespace inc {
    vector<uint8_t>hashKey(const long long& n) {
        vector<uint8_t>key;
        long long m = n;
        if (!m)
            key.push_back(uint8_t(0));
        while (m) {
            key.push_back(uint8_t(m % 256));
            m /= 256;
        }
        return key;
    }

    vector<uint8_t> hashKey(const string& s) {
        vector<uint8_t>key;
        for (const char& ch : s)
            key.push_back(uint8_t(ch));
        return key;
    }

    vector<uint8_t> hashKey(const Date& d) {
        vector<uint8_t>key;
        for (char ch : d.getDate())
            key.push_back(uint8_t(ch));
        return key;
    }
}
#endif
