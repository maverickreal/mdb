#include "tests.h"
#include "filesystem"
#include "mdb/mdb.h"
#include "string"

/* Healthy reminder -> https://www.geeksforgeeks.org/namespace-in-c/
 * set fs namespace to filesystem from std
*/

namespace fs = std::filesystem;

/* TEST_CASE(s1, s2){testCaseDefinition}
 * REQUIRE() -> assert
 * ALSO see -> https://codingnest.com/the-little-things-testing-with-catch-2/
 * s1 -> name of the test case, s2 -> tag for the test case
*/

TEST_CASE("Create a fresh database", "[createFreshDB]") {
    SECTION("Default configuration") {
        std::string dbName;
        std::cout << "Enter database name\t:\n";
        std::cin >> dbName;
        std::unique_ptr<Idatabase> db = mdb::Mdb::createFreshDB(dbName);

        /* status(o) -> returns info about type and attributes of the fs object o
         * https://en.cppreference.com/w/cpp/filesystem/directory_iterator
        */

        REQUIRE(fs::is_directory(fs::status(db->getDirectory())));
        const auto& p = fs::directory_iterator(db->getDirectory());
        REQUIRE(p == fs::end(p));
        bool fl;
        std::cout << "\nDelete the db?\n";
        std::cin >> fl;
        if (fl)
            db->destroy();
    }
}

TEST_CASE("lOAD A DATABASE", "[loadDB]") {
    SECTION("Default configuration") {
        std::string dbName;
        std::cout << "Enter database name\t:\n";
        std::cin >> dbName;
        std::unique_ptr<Idatabase> db(mdb::Mdb::loadDB(dbName));
        REQUIRE(fs::is_directory(fs::status(db->getDirectory())));
        bool fl;
        std::cout << "\nDelete the db?\n";
        std::cin >> fl;
        if (fl)
            db->destroy();
    }
}

TEST_CASE("Store and retrieve values", "[setKeyValue, getKeyValue]") {
    SECTION("Base get and set") {
        std::string dbName, k, v;
        std::cout << "Enter database name, key and value\t:\n";
        std::cin >> dbName >> k >> v;
        std::unique_ptr<Idatabase> db = mdb::Mdb::createFreshDB(dbName);
        db->setKeyValue(k, v);
        std::cout << "\nPrinting key-value\t:\t" << k << '\t' << db->getKeyValue(k) << '\n';
        REQUIRE(v == db->getKeyValue(k));
        bool fl;
        std::cout << "\nDelete the db?\n";
        std::cin >> fl;
        if (fl)
            db->destroy();
    }
}
