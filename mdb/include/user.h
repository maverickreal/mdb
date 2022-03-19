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

namespace inc {

    unique_ptr<Idatabase>usersDB;

    class User :public IUser {
    private:
        class impl :public IUser {
        private:
            string memberName,
                memberPassword,
                memberId;

            bool memberAdmin;

        public:

            impl(const string& name, const string& password, bool admin) :memberName(name), memberPassword(password), memberAdmin(admin) {
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

            static unique_ptr<IUser> createEmpty(const string& name, const string& password, bool admin) {
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
                if (usersDB->keyExists(name + " " + password))
                    throw "User already exists!\n";
                usersDB->setKeyValue(name + " " + password, admin ? "1" : "0");
                return make_unique<impl>(name, password, admin);
            }

            static unique_ptr<IUser> load(const string& name, const string& password) {
                if (!usersDB) {
                    try {
                        usersDB = databaseEmbedded::createEmpty(rootDir, userDB);
                    }
                    catch (...) {
                        usersDB = databaseEmbedded::load(rootDir, userDB);
                    }
                }
                if (!usersDB->keyExists(name + " " + password))
                    throw "User not found!\n";
                return make_unique<impl>(name, password, usersDB->getKeyValue(name + " " + password) == "1");
            }

            string getId() const {
                return memberId;
            }

            string getName() const {
                return memberName;
            }

            string getPassword() const {
                return memberPassword;
            }

            bool isAdmin() const {
                return memberAdmin;
            }

            void setId(const string& id) {
                memberId = id;
            }

            void setName(const string& name) {
                memberName = name;
            }

            void setPassword(const string& password) {
                memberPassword = password;
            }

            void setAdmin(bool admin) {
                memberAdmin = admin;
            }

            void destroy() {
                usersDB->removeKeyValue(memberId);
            }
        };

        unique_ptr<impl> memberImpl;

    public:
        User() {}

        ~User() {}

        unique_ptr<Idatabase>createFreshDB(const string& dbName)const {
            return memberImpl->createFreshDB(dbName);
        }

        unique_ptr<Idatabase>loadDB(const string& dbName)const {
            return memberImpl->loadDB(dbName);
        }

        static unique_ptr<IUser> load(const string& name, const string& password) {
            return impl::load(name, password);
        }

        static unique_ptr<IUser> createEmpty(const string& name, const string& password, bool admin) {
            return impl::createEmpty(name, password, admin);
        }

        void setName(const string& name) {
            memberImpl->setName(name);
        }

        void setPassword(const string& password) {
            return memberImpl->setPassword(password);
        }

        void setAdmin(bool admin) {
            memberImpl->setAdmin(admin);
        }

        string getName()const {
            return memberImpl->getName();
        }

        string getPassword()const {
            return memberImpl->getPassword();
        }

        bool isAdmin()const {
            return memberImpl->isAdmin();
        }

        string getId()const {
            return memberImpl->getId();
        }

        void setId(const string& id) {
            memberImpl->setId(id);
        }

        void destroy() {
            memberImpl->destroy();
        }
    };
}
#endif