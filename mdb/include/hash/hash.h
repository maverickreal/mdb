#ifndef HASH_H
#define HASH_H

#include "xxhash.h"
#include "string"
#include "highwayhash.h"

using namespace std;
using namespace inc;

namespace customHash {
    class highwayHash {
    public:
        uint64_t operator()(const double& data)const {
            string str = to_string(data);
            HighwayHashState state;
            ProcessAll((uint8_t*)str.data(), str.length(), &state);
            return HighwayHashFinalize64(&state);
        }

        uint64_t operator()(const string& data)const {
            HighwayHashState state;
            ProcessAll((uint8_t*)data.data(), data.length(), &state);
            return HighwayHashFinalize64(&state);
        }

        uint64_t operator()(const Date& data)const {
            HighwayHashState state;
            ProcessAll((uint8_t*)data.getDate().data(), data.getDate().length(), &state);
            return HighwayHashFinalize64(&state);
        }
    };

    class xxHash {
    public:
        uint64_t operator()(const double& data)const {
            const string str = to_string(data);
            return XXHash64::hash(&str, str.length(), 7);
        }

        uint64_t operator()(const string& data)const {
            return XXHash64::hash(data.data(), data.length(), 7);
        }

        uint64_t operator()(const Date& data)const {
            const string str = data.getDate();
            return XXHash64::hash(&str, str.length(), 7);
        }
    };
}
#endif
