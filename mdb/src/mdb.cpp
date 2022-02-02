#include "mdb.h"
using namespace std;

/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
                      https://www.geeksforgeeks.org/scope-resolution-operator-in-c */

Mdb::Mdb()
{
}

database Mdb::createFreshDB(const std::string& dbName) {
    return database::createEmpty(dbName);
}

database Mdb::loadDB(const std::string& dbName) {
    return database::load(dbName);
}
