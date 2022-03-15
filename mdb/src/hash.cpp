#include "include/hash.h"

using namespace inc;

pair<uint8_t*, int> hashKey(const long long& n) {
    vector<uint8_t>key;
    long long m = n;
    if (!m)
        key.push_back(uint8_t(0));
    while (m) {
        key.push_back(uint8_t(m % 256));
        m /= 256;
    }
    return { key.data(), key.size() };
}

pair<uint8_t*, int> hashKey(const string& s) {
    vector<uint8_t>key;
    for (const char& ch : s)
        key.push_back(uint8_t(ch));
    return { key.data(), key.size() };
}

pair<uint8_t*, int> hashKey(const Date& d) {
    vector<uint8_t>key;
    for (char ch : d.getDate())
        key.push_back(uint8_t(ch));
    return { key.data(), key.size() };
}