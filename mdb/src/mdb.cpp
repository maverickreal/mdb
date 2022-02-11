#include "mdb.h"
using namespace std;

/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
                      https://www.geeksforgeeks.org/scope-resolution-operator-in-c */

Mdb::Mdb()
{
}

databaseEmbedded Mdb::createFreshDB(const std::string& dbName) {
    return databaseEmbedded::createEmpty(dbName);
}

databaseEmbedded Mdb::loadDB(const std::string& dbName) {
    return databaseEmbedded::load(dbName);
}
