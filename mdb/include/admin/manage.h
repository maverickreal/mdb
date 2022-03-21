#ifndef MANAGE_H
#define MANAGE_H

#include "string"
#include "database.h"
#include "mdb.h"
#include "list"

using namespace std;
using namespace inc;

namespace manage {
    class Manage {
    public:

        static void createAdmin(const string& name, const string& password) {
            unique_ptr<IUser> user;
            try {
                user = Mdb::loadUser(name, password);
            }
            catch (...) {
                user = Mdb::createFreshUser(name, password);
            }
            usersDB->setKeyValue(name + " " + password, "1");
            user->sync();
        }

        static void removeAdmin(const string& name, const string& password) {
            try {
                unique_ptr<IUser> user = Mdb::loadUser(name, password);
                usersDB->setKeyValue(name + " " + password, "0");
                user->sync();
            }
            catch (...) {
                cout << "User does not exist.\n";
                return;
            }
        }

        static list<pair<string, string>>& getUsers() {
            return usersDB->getKeysAndValues();
        }
    };
}

#endif