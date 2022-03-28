#ifndef USER_H
#define USER_H

#include "string"
#include "queue"
#include "filesystem"
#include "stores.h"
#include "database.h"

using namespace std;
using namespace stores;
using namespace database;

static long long maxID = -1;

namespace user {

    priority_queue<long long>idQueue;
    unique_ptr<Idatabase>usersDB;

    class User : public IUser {
    private:
        class impl : public IUser {
        private:

            string memberName,
                memberPassword,
                memberId;

            bool memberAdmin;

        public:

            impl(const string& name, const string& password) :memberName(name), memberPassword(password) {
                long long newId;
                if (idQueue.empty()) {
                    newId = ++maxID;
                }
                else {
                    newId = idQueue.top();
                    idQueue.pop();
                }
                memberId = Crypt::encrypt(name + " " + password + " " + to_string(newId));
            }

            ~impl() {}

            unique_ptr<Idatabase> createFreshDB(const string& dbName)const {
                return databaseEmbedded::createEmpty(dbName, memberName + " " + memberPassword);
            }

            unique_ptr<Idatabase> loadDB(const string& dbName)const {
                return databaseEmbedded::load(dbName, memberName + " " + memberPassword);
            }

            static void create(const string& name, const string& password) {
                if (usersDB->keyExists(name + " " + password))
                    throw "User already exists!\n";
                usersDB->setKeyValue(name + " " + password, "0");
            }

            static void load(const string& name, const string& password) {
                if (!usersDB->keyExists(name + " " + password))
                    throw "User not found!\n";
            }

            // Refreshes the memory store with respect to the file store.
            void sync() {
                memberAdmin = (usersDB->getKeyValue(memberName + " " + memberPassword) == "1");
            }

            void setName(const string& name) {
                memberName = name;
            }

            void setPassword(const string& oldPassword, const string& password) {
                if (oldPassword != memberPassword)
                    throw "Wrong password!\n";
                memberPassword = password;
            }

            void destroy() {
                usersDB->removeKeyValue(memberId);
            }
        };
        unique_ptr<impl> memberImpl;

    public:
        User(const string& name, const string& password, const bool& load) {
            if (!usersDB) {

                if (!filesystem::exists(rootDir))
                    filesystem::create_directory(rootDir);

                try {
                    usersDB = databaseEmbedded::load(userDB, internal_user);
                }

                catch (...) {
                    usersDB = databaseEmbedded::createEmpty(userDB, internal_user);
                }
            }
            if (!load && usersDB->keyExists(name + " " + password))
                throw "User already exists!\n";

            if (load && !usersDB->keyExists(name + " " + password))
                throw "User does not exist!\n";
            memberImpl = make_unique<impl>(name, password);

            if (load)
                impl::load(name, password);
            else
                impl::create(name, password);
        }

        ~User() {}

        unique_ptr<Idatabase>createFreshDB(const string& dbName)const {
            return memberImpl->createFreshDB(dbName);
        }

        unique_ptr<Idatabase>loadDB(const string& dbName)const {
            return memberImpl->loadDB(dbName);
        }

        void sync() {
            memberImpl->sync();
        }

        void setName(const string& name) {
            memberImpl->setName(name);
        }

        void setPassword(const string& oldPassword, const string& password) {
            return memberImpl->setPassword(oldPassword, password);
        }

        void destroy() {
            memberImpl->destroy();
        }
    };
}
#endif