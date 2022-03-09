#ifndef DATABASE_H
#define DATABASE_H
#include "string"
#include "memory"
#include "functional"

namespace Mdb {

  class store {
  public:
    store() = default;
    virtual ~store() = default;
  };

  class keyValueStore : public store {
  public:
    keyValueStore() = default;
    virtual ~keyValueStore() = default;
    virtual void setKeyValue(const std::string& key, const std::string& value) = 0;
    virtual std::string getKeyValue(const std::string& key) = 0;
    // Healthy Reminder -> https://www.geeksforgeeks.org/lambda-expression-in-c/
    virtual void loadKeysInto(const std::function<void(std::string key, std::string value)>& callBack) = 0;
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

    virtual void setKeyValue(const std::string& key, const std::string& value) = 0;

    virtual std::string getKeyValue(const std::string& key) = 0;

    static const std::unique_ptr<Idatabase>createEmpty(const std::string& dbName);

    static const std::unique_ptr<Idatabase> createEmpty(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore);

    static const std::unique_ptr<Idatabase>load(const std::string& dbName);

    virtual std::string getDirectory(void) = 0;// takes no args

    // An instruction to the compiler to not privately create a constructor
  };

}
#endif
