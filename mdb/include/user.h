#ifndef USER_H
#define USER_H

#include "string"
#include "unordered_map"
#include "time.h"
#include "mdb.h"
#include "unordered_set"
#include "stores.h"

using namespace std;
using namespace customHash;
using namespace manage;

namespace inc {

    static unique_ptr<Idatabase>usersDB;

    class User :public IUser {
    private:

        class impl :public IUser {
        private:

            string memberName,
                memberPassword,
                memberId;

            bool memberAdmin;

        public:

            impl(const string& name, const string& password) :memberName(name), memberPassword(password) {
                string tmp = memberName + " " + memberPassword;
                const xxHash xxh;
                tmp = to_string(xxh(tmp));
                memberId = tmp;
            }

            ~impl() {}

            unique_ptr<Idatabase> createFreshDB(const string& dbName)const {
                string tmp = dbName;
                const xxHash xxh;
                tmp = to_string(xxh(tmp));
                return databaseEmbedded::createEmpty(tmp, memberId);
            }

            unique_ptr<Idatabase> loadDB(const string& dbName)const {
                string tmp = dbName;
                const xxHash xxh;
                tmp = to_string(xxh(tmp));
                return databaseEmbedded::load(tmp, memberId);
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

            void sync() {
                memberAdmin = usersDB->getKeyValue(memberName + " " + memberPassword) == "1";
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
                delete this;
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
                    cout << "Creating\n";
                    usersDB = databaseEmbedded::createEmpty(userDB, internal_user);
                }

            }

            if (!load && usersDB->keyExists(name + " " + password)) {
                delete this;
                throw "User already exists!\n";
            }

            if (load && !usersDB->keyExists(name + " " + password)) {
                delete this;
                throw "User does not exist!\n";
            }

            memberImpl = make_unique<impl>(name, password);

            try {
                impl::create(name, password);
            }

            catch (...) {
                impl::load(name, password);
            }
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
            delete this;
        }
    };
}
#endif