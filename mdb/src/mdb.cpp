#include "mdb.h"
#include "extensions/extension.h"

using namespace ext;
using namespace std;
using namespace inc;
/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
                      https://www.geeksforgeeks.org/scope-resolution-operator-in-c */

unique_ptr<Idatabase> createFreshDB(const string& dbName) {
    return databaseEmbedded::createEmpty(dbName);
}

unique_ptr<Idatabase> loadDB(const string& dbName) {
    return databaseEmbedded::load(dbName);
}
