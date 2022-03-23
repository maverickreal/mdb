#ifndef ADMIN_H 
#define ADMIN_H

#include "list"
#include "string"
#include "mdb.h"
#include "database.h"
#include "user.h"

using namespace std;
using namespace user;
using namespace mdb;
using namespace database;

namespace admin {
    void createAdmin(const string& name, const string& password) {
        unique_ptr<IUser> user;
        try {
            user = loadUser(name, password);
        }
        catch (...) {
            user = createFreshUser(name, password);
        }
        usersDB->setKeyValue(name + " " + password, "1");
        user->sync();
    }

    void removeAdmin(const string& name, const string& password) {
        try {
            unique_ptr<IUser> user = loadUser(name, password);
            usersDB->setKeyValue(name + " " + password, "0");
            user->sync();
        }
        catch (...) {
            cout << "User does not exist.\n";
            return;
        }
    }

    list<pair<string, string>> getUsers() {
        return usersDB->getKeysAndValues();
    }
}
#endif