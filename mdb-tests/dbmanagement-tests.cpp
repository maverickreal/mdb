#include "tests.h"
#include "filesystem"
#include "../mdb/include/mdb.h"
#include "string"

/* Healthy reminder -> https://www.geeksforgeeks.org/namespace-in-c/
 * set fs namespace to filesystem from std
*/
using namespace inc;
using namespace std;

/* TEST_CASE(s1, s2){testCaseDefinition}
 * REQUIRE() -> assert
 * ALSO see -> https://codingnest.com/the-little-things-testing-with-catch-2/
 * s1 -> name of the test case, s2 -> tag for the test case
*/

TEST_CASE("Create a fresh database", "[createFreshDB]") {
    SECTION("Default configuration") {
        string dbName;
        cout << "Enter database name\t:\n";
        cin >> dbName;
        auto db = Mdb::createFreshDB(dbName);

        /* status(o) -> returns info about type and attributes of the fs object o
         * https://en.cppreference.com/w/cpp/filesystem/directory_iterator
        */

        REQUIRE(filesystem::is_directory(filesystem::status(db->getDirectory())));
        const auto& p = filesystem::directory_iterator(db->getDirectory());
        REQUIRE(p == filesystem::end(p));
        bool fl;
        cout << "\nDelete the db?\n";
        cin >> fl;
        if (fl)
            db->destroy();
    }
}

TEST_CASE("Load a database", "[loadDB]") {
    SECTION("Default configuration") {
        string dbName;
        cout << "Enter database name\t:\n";
        cin >> dbName;
        auto db = Mdb::loadDB(dbName);
        REQUIRE(filesystem::is_directory(filesystem::status(db->getDirectory())));
        bool fl;
        cout << "\nDelete the db?\n";
        cin >> fl;
        if (fl)
            db->destroy();
    }
}

TEST_CASE("Store and retrieve values", "[setKeyValue, getKeyValue]") {
    SECTION("Base get and set") {
        string dbName, k, v;
        cout << "Enter database name, key and value\t:\n";
        cin >> dbName >> k >> v;
        auto db = Mdb::createFreshDB(dbName);
        db->setKeyValue(k, v);
        cout << "\nPrinting key-value\t:\t" << k << '\t' << db->getKeyValue(k) << '\n';
        REQUIRE(v == db->getKeyValue(k));
        bool fl;
        cout << "\nDelete the db?\n";
        cin >> fl;
        if (fl)
            db->destroy();
    }
}
