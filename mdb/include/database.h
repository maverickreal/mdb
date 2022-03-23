#ifndef DATABASE_H
#define DATABASE_H

#include "string"
#include "functional"
#include "list"
#include "types.h"

using namespace std;
using namespace types;

namespace database {

  class keyValueStore {
  public:

    keyValueStore() = default;

    virtual ~keyValueStore() = default;

    virtual void setKeyValue(const string& key, const string& value) = 0;

    virtual void setKeyValue(const Date& key, const string& value) = 0;

    virtual void setKeyValue(const double& key, const string& value) = 0;

    virtual string getKeyValue(const string& key) = 0;

    virtual string getKeyValue(const Date& key) = 0;

    virtual string getKeyValue(const double& key) = 0;

    virtual bool keyExists(const string& key) = 0;

    virtual bool keyExists(const Date& key) = 0;

    virtual bool keyExists(const double& key) = 0;

    virtual void removeKeyValue(const string& key) = 0;

    virtual void removeKeyValue(const Date& key) = 0;

    virtual void removeKeyValue(const double& key) = 0;

    // Healthy Reminder -> https://www.geeksforgeeks.org/lambda-expression-in-c/
    virtual void loadKeysInto(const function<void(const string& key, const string& value)>& callBack) = 0;

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

    virtual void setKeyValue(const Date& key, const string& value) = 0;

    virtual void setKeyValue(const double& key, const string& value) = 0;

    virtual string getKeyValue(const string& key) = 0;

    virtual string getKeyValue(const Date& key) = 0;

    virtual string getKeyValue(const double& key) = 0;

    virtual bool keyExists(const string& key) = 0;

    virtual bool keyExists(const Date& key) = 0;

    virtual bool keyExists(const double& key) = 0;

    virtual void removeKeyValue(const string& key) = 0;

    static const unique_ptr<Idatabase>createEmpty(const string& dbName);

    static const unique_ptr<Idatabase>load(const string& dbName);

    virtual string getDirectory(void) = 0;// takes no args

    virtual list<pair<string, string>> getKeysAndValues() = 0;
  };

  class IUser {
  public:
    IUser() = default;

    virtual ~IUser() = default;

    virtual unique_ptr<Idatabase>createFreshDB(const string& dbName)const = 0;

    virtual unique_ptr<Idatabase>loadDB(const string& dbName)const = 0;

    virtual void destroy() = 0;

    virtual void sync() = 0;

    virtual void setName(const string& name) = 0;

    virtual void setPassword(const string& oldPassword, const string& password) = 0;
  };
}
#endif