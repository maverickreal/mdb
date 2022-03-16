#ifndef DATABASE_H
#define DATABASE_H

#include "string"
#include "memory"
#include "functional"

using namespace std;

namespace inc {

  class store {
  public:

    store() = default;

    virtual ~store() = default;
  };

  class keyValueStore : public store {
  public:

    keyValueStore() = default;

    virtual ~keyValueStore() = default;

    virtual void setKeyValue(const string& key, const string& value) = 0;

    virtual string getKeyValue(const string& key) = 0;

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

    static const unique_ptr<Idatabase>createEmpty(const string& dbName);

    static const unique_ptr<Idatabase>load(const string& dbName);

    virtual string getDirectory(void) = 0;// takes no args
  };

}
#endif