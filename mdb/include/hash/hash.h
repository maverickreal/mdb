#ifndef HASH_H
#define HASH_H

#include "xxhash.h"
#include "string"
#include "highwayhash.h"

using namespace std;

namespace hashes {
    class highwayHash {
    public:

        uint64_t operator()(const string& data)const {
            HighwayHashState state;
            ProcessAll((uint8_t*)data.data(), data.length(), &state);
            return HighwayHashFinalize64(&state);
        }
    };

    class xxHash {
    public:

        uint64_t operator()(const string& data)const {
            return XXHash64::hash(data.data(), data.length(), 7);
        }

    };
}
#endif
