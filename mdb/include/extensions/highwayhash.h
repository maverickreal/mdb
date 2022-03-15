#include "string"

using namespace std;

namespace ext {

    class HighwayHashState {
    public:
        uint64_t v0[4];

        uint64_t v1[4];

        uint64_t mul0[4];

        uint64_t mul1[4];
    };

    class highwayHash {
    public:

        uint64_t operator()(const string& data)const;

        uint64_t operator()(const long long& data)const;

        void operator++() {}

        void operator*() {}

    private:
        static void ProcessAll(const uint8_t* data, size_t size, const uint64_t key[4],
            HighwayHashState* state);

        static void PermuteAndUpdate(HighwayHashState* state);

        static void Permute(const uint64_t v[4], uint64_t* permuted);

        static void Rotate32By(uint64_t count, uint64_t lanes[4]);

        static uint64_t Read64(const uint8_t* src);

        static void Update(const uint64_t lanes[4], HighwayHashState* state);

        static void ZipperMergeAndAdd(const uint64_t v1, const uint64_t v0,
            uint64_t* add1, uint64_t* add0);

        static void HighwayHashReset(const uint64_t key[4], HighwayHashState* state);

        static void HighwayHashUpdatePacket(const uint8_t* packet, HighwayHashState* state);

        static void HighwayHashUpdateRemainder(const uint8_t* bytes, const size_t size_mod32,
            HighwayHashState* state);

        static uint64_t HighwayHashFinalize64(HighwayHashState* state);
    };
}