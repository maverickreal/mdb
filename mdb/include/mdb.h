#ifndef MDB_H
#define MDB_H

#include "string"
#include "stores.h"
#include "user.h"

using namespace std;

namespace inc {
    class Mdb {
    public:
        /* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
        https://www.geeksforgeeks.org/scope-resolution-operator-in-c
        */
        static unique_ptr<IUser> createFreshUser(const string& name, const string& password) {
            return make_unique<IUser>(name, password, false);
        }

        static unique_ptr<IUser> loadUser(const string& name, const string& password) {
            return make_unique<IUser>(name, password, true);
        }
    };
}
#endif
