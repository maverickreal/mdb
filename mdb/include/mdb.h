#ifndef MDB_H
#define MDB_H

#include "string"
#include "stores.h"

using namespace std;

namespace inc {
    class Mdb {
    public:
        /* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
        https://www.geeksforgeeks.org/scope-resolution-operator-in-c
        */
        static unique_ptr<Idatabase> createFreshDB(const string& dbName) {
            return databaseEmbedded::createEmpty(dbName);
        }

        static unique_ptr<Idatabase> loadDB(const string& dbName) {
            return databaseEmbedded::load(dbName);
        }

    };
}
#endif
