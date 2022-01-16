#include "tests.h"
#include "filesystem"
#include "mdb/mdb.h"
#include "string"
#include "mdb/database.h"
/* Healthy reminder -> https://www.geeksforgeeks.org/namespace-in-c/
 * set fs namespace to filesystem from std
*/
namespace fs=std::filesystem;

/* TEST_CASE(s1, s2){testCaseDefinition}
 * REQUIRE() -> assert
 * ALSO see -> https://codingnest.com/the-little-things-testing-with-catch-2/
 * s1 -> name of the test case, s2 -> tag for the test case
*/
TEST_CASE("Create a fresh database", "[createFreshDB]"){
    SECTION("Default configuration"){
        std::string dbName("freshDB");
        database db(Mdb::createFreshDB(dbName));

        /* status(o) -> returns info about type and attributes of the fs object o
         * https://en.cppreference.com/w/cpp/filesystem/directory_iterator
        */
        REQUIRE(fs::is_directory(fs::status(db.getDirectory())));
        const auto&p=fs::directory_iterator(db.getDirectory());
        Require(p==end(p));
    }
}
