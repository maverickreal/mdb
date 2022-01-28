#include "catch.hpp"
#include "mdb/mdb.h"
#include "mdb/database.h"

TEST_CASE("Store and retrieve values", "[setKeyValue, getKeyValue]"){
    SECTION("Base get and set"){
        std::string dbName="fp", k="key", v="sigma";
        database db(Mdb::createFreshDB(dbName));
        db.setKeyValue(k,v);
        REQUIRE(v==db.getKeyValue(k));
        db.destroy();
    }
}
