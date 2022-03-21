#ifndef DATABASE_H
#define DATABASE_H

#include "string"
#include "memory"
#include "functional"

using namespace std;

namespace inc {

  class keyValueStore {
  public:

    keyValueStore() = default;

    virtual ~keyValueStore() = default;

    virtual void setKeyValue(const string& key, const string& value) = 0;

    virtual string getKeyValue(const string& key) = 0;

    virtual bool keyExists(const string& key) = 0;

    virtual void removeKeyValue(const string& key) = 0;

    // Healthy Reminder -> https://www.geeksforgeeks.org/lambda-expression-in-c/
    virtual void loadKeysInto(const function<void(string key, string value)>& callBack) = 0;

    virtual void clear() = 0;
  };

  class Idatabase {
  public:
    // Healthy reminder -> https://www.geeksforgeeks.org/virtual-function-cpp/
    //                     https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/
    //                     https://www.geeksforgeeks.org/explicitly-defaulted-deleted-functions-c-11/
    Idatabase() = default;

    virtual ~Idatabase() = default;

    virtual void destroy() = 0;

    virtual void setKeyValue(const string& key, const string& value) = 0;

    virtual string getKeyValue(const string& key) = 0;

    virtual bool keyExists(const string& key) = 0;

    virtual void removeKeyValue(const string& key) = 0;

    static const unique_ptr<Idatabase>createEmpty(const string& dbName);

    static const unique_ptr<Idatabase>load(const string& dbName);

    virtual string getDirectory(void) = 0;// takes no args
  };

  class IUser {
  public:
    IUser() = default;

    virtual ~IUser() = default;

    virtual unique_ptr<Idatabase>createFreshDB(const string& dbName)const = 0;

    virtual unique_ptr<Idatabase>loadDB(const string& dbName)const = 0;

    virtual void destroy() = 0;

    virtual void setName(const string& name) = 0;

    virtual string getName()const = 0;

    virtual void setPassword(const string& password) = 0;

    virtual string getPassword()const = 0;

    virtual void setAdmin(bool admin) = 0;

    virtual bool isAdmin()const = 0;

    virtual string getId()const = 0;

    virtual void setId(const string& id) = 0;
  };
}
#endif
