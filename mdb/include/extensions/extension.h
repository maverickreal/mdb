#ifndef extension_H
#define extension_h
#include "database.h"
#include "../database.h"
#include "functional"
using namespace Mdb;
namespace mdbExt {

  class memoryKeyValueStore : public keyValueStore {
  public:
    memoryKeyValueStore();
    memoryKeyValueStore(std::unique_ptr<keyValueStore>& toCache);
    ~memoryKeyValueStore();
    void setKeyValue(const std::string& key, const std::string& value);
    std::string getKeyValue(const std::string& key);
    void loadKeysInto(const std::function<void(std::string key, std::string value)>& callBack);
    void clear();

  private:
    class impl;
    std::unique_ptr<impl> memberImpl;
  };

  class fileKeyValueStore : public keyValueStore {
  public:
    fileKeyValueStore(const std::string& fullpath);
    ~fileKeyValueStore();
    void setKeyValue(const std::string& key, const std::string& value);
    std::string getKeyValue(const std::string& key);
    void loadKeysInto(const std::function<void(std::string key, std::string value)>& callBack);
    void clear();

  private:
    class impl;
    std::unique_ptr<impl> memberImpl;
  };

  class databaseEmbedded : public Idatabase {
  public:
    databaseEmbedded(const std::string& dbName, const std::string& fullPath);

    ~databaseEmbedded();

    databaseEmbedded(const std::string& dbname, const std::string& fullpath, std::unique_ptr<keyValueStore>& keyValueStore);

    static const std::unique_ptr<Idatabase> createEmpty(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore);

    void destroy();

    void setKeyValue(const std::string& key, const std::string& value);

    std::string getKeyValue(const std::string& key);

    static std::unique_ptr<Idatabase> createEmpty(const std::string& dbName);

    static std::unique_ptr<Idatabase> load(const std::string& dbName);

    std::string getDirectory(void);

    class impl;
  private:
    // Healthy reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/
    std::unique_ptr<impl>memberImpl;
  };
}
#endif
