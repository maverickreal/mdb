/* #include "catch.hpp"
#include "../mdb/mdb.h"
#include "../mdb/include/extensions/extension.h"
#include "../mdb/highwayhash/highwayhash.h"
#include "unordered_map"
#include "../mdb/include/extensions/highwayhash.h"

//using namespace highwayHash; -----------------> I M P O R T A N T
using namespace mdb;
using namespace std;

TEST_CASE("Hashing", "[set,get]") {
    SECTION("Produces known expected value for known input") {
        highwayHash h;
        string text = "Known";
        size_t r = h(text);
        HHKey key HH_ALIGNAS(64) = { 1, 2, 3, 4 };
        HHResult64 result;  // 64, 128, or 256
        HHStateT<HH_TARGET> state(key);
        HighwayHashT(&state, text.c_str(), text.length(), &result);

        REQUIRE(result == r);
    }

    SECTION("Previous hash doesn't affect next hash result") {
        highwayHash hFirst;
        size_t rFirst = hFirst(string("OtherThing"));

        highwayHash h;
        size_t r1 = h(string("Known")), r2 = h(string("OtherThing"));

        REQUIRE(r1 != r2);
        REQUIRE(r1 != rFirst);
        REQUIRE(rFirst == r2);
    }

    SECTION("Different seed keys produce different results for the same input") {
        highwayHash h1{ 1,2,3,4 };
        size_t r1 = h1(string("AThing"));

        highwayHash h2{ 5,6,7,8 };
        size_t r2 = h2(string("AThing"));

        REQUIRE(r1 != r2);
    }

    SECTION("Unordered map works as expected with custom hash") {
        unordered_map<string, string, highwayHash>ump;
        string key("A very sensible key name"),
            value("A very valuable value"),
            k2("Some other key"),
            v2("Another value");
        ump.emplace(key, value);
        ump.emplace(k2, v2);

        REQUIRE(ump[key] == value);
        REQUIRE(ump[k2] == v2);
    }

    SECTION("StringKeyHashing") {
        mdb::key<string> k1("somekey"),
            k2("somekey"),
            k3("someotherkey");
        string buffer1("somekey"),
            buffer2("somekey"),
            buffer3("someotherkey");
        highwayHash h1{ 1,2,3,4 };
        size_t hv1 = h1(buffer1),
            hv2 = h1(buffer2),
            hv3 = h1(buffer3);

        REQUIRE(hv1 == hv2);
        REQUIRE(hv1 != hv3);
        REQUIRE(hv2 != hv3);
        REQUIRE(8 == sizeof(hv1));
        REQUIRE(8 == sizeof(hv2));
        REQUIRE(8 == sizeof(hv3));
    }
}
 */