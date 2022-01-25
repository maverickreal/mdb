#include "catch.hpp"
#include "mdb/mdb.h"
#include "mdb/database.h"

TEST_CASE("Store and retrieve values", "[setKeyValue, getKeyValue]"){
    SECTION("Base get and set"){
        std::string dbName("fp");
        database db(Mdb::createFreshDB(dbName, "fp"));
        std::string k("key");
        std::string v("sigma");
        db.setKeyValue(k,v);
        REQUIRE(v==db.getKeyValue(k));
    }
}
