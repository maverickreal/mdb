#ifndef MDB_H
#define MDB_H

#include "string"
#include "user.h"
#include "database.h"

using namespace user;
using namespace database;

/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
https://www.geeksforgeeks.org/scope-resolution-operator-in-c
*/
namespace mdb {
    unique_ptr<IUser> createFreshUser(const string& name, const string& password) {
        return make_unique<User>(name, password, false);
    }

    unique_ptr<IUser> loadUser(const string& name, const string& password) {
        return make_unique<User>(name, password, true);
    }
}
#endif
