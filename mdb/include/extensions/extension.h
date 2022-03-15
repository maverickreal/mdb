#ifndef extension_H
#define extension_h
#include "database.h"
#include "functional"

using namespace std;
using namespace inc;

namespace ext {

  class memoryKeyValueStore : public keyValueStore {
  public:
    memoryKeyValueStore();

    memoryKeyValueStore(unique_ptr<keyValueStore>& toCache);

    ~memoryKeyValueStore();

    void setKeyValue(const string& key, const string& value);

    string getKeyValue(const string& key);

    void loadKeysInto(const function<void(string key, string value)>& callBack);

    void clear();

  private:
    class impl;

    unique_ptr<impl> memberImpl;
  };

  class fileKeyValueStore : public keyValueStore {
  public:

    fileKeyValueStore(const string& fullpath);

    ~fileKeyValueStore();

    void setKeyValue(const string& key, const string& value);

    string getKeyValue(const string& key);

    void loadKeysInto(const function<void(string key, string value)>& callBack);

    void clear();

  private:
    class impl;

    unique_ptr<impl> memberImpl;
  };

  class databaseEmbedded : public Idatabase {
  public:

    databaseEmbedded(const string& dbName, const string& fullPath);

    ~databaseEmbedded();

    databaseEmbedded(const string& dbname, const string& fullpath, unique_ptr<keyValueStore>& keyValueStore);

    static const unique_ptr<Idatabase> createEmpty(const string& dbname, unique_ptr<keyValueStore>& keyValueStore);

    void destroy();

    void setKeyValue(const string& key, const string& value);

    string getKeyValue(const string& key);

    static unique_ptr<Idatabase> createEmpty(const string& dbName);

    static unique_ptr<Idatabase> load(const string& dbName);

    string getDirectory(void);

    class impl;
  private:
    // Healthy reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/
    unique_ptr<impl>memberImpl;
  };
}
#endif
