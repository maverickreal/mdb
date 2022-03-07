#include "mdb.h"
#include "extensions/extension.h"
using namespace mdbExt;

/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
                      https://www.geeksforgeeks.org/scope-resolution-operator-in-c */

mdb::Mdb::Mdb() { ; }

std::unique_ptr<Idatabase> mdb::Mdb::createFreshDB(const std::string& dbName) {
    return databaseEmbedded::createEmpty(dbName);
}

std::unique_ptr<Idatabase> mdb::Mdb::loadDB(const std::string& dbName) {
    return databaseEmbedded::load(dbName);
}
