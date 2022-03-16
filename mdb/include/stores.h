#ifndef EXTENSION_H
#define EXTENSION_H

#include "database.h"
#include "filesystem"
#include "functional"
#include "fstream"
#include "iostream"
#include "optional"
#include "highwayhash.h"

/* Healthy reminder -> https://www.cplusplus.com/reference/filesystemtream/ofilesystemtream/
https://www.cplusplus.com/reference/memory/unique_ptr/
https://www.cplusplus.com/reference/filesystemtream/ios/
https://www.cplusplus.com/reference/memory/unique_ptr/release/
ios::in allows input (read operations) from a stream
ios::out allows output (write operations) to a stream
ios::trunc similar to out, but strictly replaces the entire content
| (bitwise OR) for flags
https://www.cplusplus.com/reference/filesystemtream/ifilesystemtream/
https://www.cplusplus.com/reference/istream/istream/tellg/
https://www.cplusplus.com/reference/istream/istream/seekg/
https://www.cplusplus.com/reference/iterator/istreambuf_iterator/
https://www.cplusplus.com/reference/memory/unique_ptr/get/
A stream is a FIFO data structure. A buffer is random access.
A buffer is an array which contains some (raw) data, a stream is an abstraction of a buffer ( each stream has a buffer ).
A buffer is a region of memory used to temporarily hold data while it is being moved from one place to another.
A stream is a source or sink of data, usually individual bytes or characters.
ios and istreambuf_iterator operate upon stream and buffer resp.
*/

using namespace std;
using namespace inc;

namespace inc {

  class memoryKeyValueStore : public keyValueStore {
  public:
    memoryKeyValueStore() : memberImpl(make_unique<memoryKeyValueStore::impl>()) {}

    memoryKeyValueStore(unique_ptr<keyValueStore>& toCache) : memberImpl(make_unique<memoryKeyValueStore::impl>(toCache)) {
      memberImpl->memberCachedStore->get()->loadKeysInto([this](const string& key, const string& value) {
        memberImpl->memberKeyValueStore[key] = value;
        });
    }

    ~memoryKeyValueStore() {}

    void setKeyValue(const string& key, const string& value) {
      memberImpl->memberKeyValueStore[key] = value;
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->setKeyValue(key, value);
    }

    string getKeyValue(const string& key) {
      const auto& v = memberImpl->memberKeyValueStore.find(key);
      return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
    }

    void loadKeysInto(const function<void(string key, string value)>& callBack) {
      for (const auto& element : memberImpl->memberKeyValueStore)
        callBack(element.first, element.second);
    }

    void clear() {
      memberImpl->memberKeyValueStore.clear();
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->clear();
    }


  private:
    class impl {
    public:

      impl() {}

      impl(unique_ptr<keyValueStore>& toCache) :memberCachedStore(toCache.release()) {}

      unordered_map<string, string, highwayHash> memberKeyValueStore;

      optional<unique_ptr<keyValueStore>> memberCachedStore;
    };

    unique_ptr<impl> memberImpl;
  };

  class fileKeyValueStore : public keyValueStore {
  public:

    fileKeyValueStore(const string& fullpath) : memberImpl(make_unique<fileKeyValueStore::impl>(fullpath)) {
      if (!filesystem::exists(fullpath))
        filesystem::create_directory(fullpath);
    }

    ~fileKeyValueStore() {}

    void setKeyValue(const string& key, const string& value) {
      ofstream os;
      os.open(memberImpl->memberFullPath + "/" + key + "-string.kv", ios::out | ios::trunc);
      os << value;
      os.close();
    }

    string getKeyValue(const string& key) {
      ifstream t(memberImpl->memberFullPath + "/" + key + "-string.kv");
      string value;
      t.seekg(0, ios::end);
      value.reserve(t.tellg());
      t.seekg(0, ios::beg);
      value.assign((istreambuf_iterator<char>(t)),
        istreambuf_iterator<char>());
      return value;
    }


    void loadKeysInto(const function<void(string key, string value)>& callBack) {
      for (const auto& p : filesystem::directory_iterator(memberImpl->memberFullPath)) {
        if (p.exists() && p.is_regular_file()) {
          if (".kv" == p.path().extension()) {
            string key = p.path().filename();
            int cnt = 10;
            while (cnt-- && !key.empty())
              key.pop_back();
            ifstream t(p.path());
            string value;
            t.seekg(0, ios::end);
            value.reserve(t.tellg());
            t.seekg(0, ios::beg);
            value.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
            callBack(key, value);
          }
        }
      }
    }

    void clear() {
      if (filesystem::exists(memberImpl->memberFullPath))
        filesystem::remove_all(memberImpl->memberFullPath);
    }

  private:
    class impl {
    public:
      impl(const string& fullpath) {
        memberFullPath = fullpath;
      }
      string memberFullPath;
    };

    unique_ptr<impl> memberImpl;
  };

  class databaseEmbedded : public Idatabase {
  private:
    class impl : public Idatabase {
    public:
      impl(const string& dbName, const string& dbDirPath) : memberName(dbName), memberFullPath(dbDirPath) {
        unique_ptr<keyValueStore> fileStore = make_unique<fileKeyValueStore>(dbDirPath),
          memoryStore = make_unique<memoryKeyValueStore>(fileStore);
        memberKeyValueStore = move(memoryStore);
      }

      impl(const string& dbname, const string& dbDirPath, unique_ptr<keyValueStore>& keyValueStore) :memberKeyValueStore(keyValueStore.release()) {
        memberName = dbname;
        memberFullPath = dbDirPath;
      }

      ~impl() {}

      static const unique_ptr<Idatabase> createEmpty(const string& dbName) {

        if (!filesystem::exists(".mdb"))
          filesystem::create_directory(".mdb");

        const string dbDir = ".mdb/" + dbName;

        if (filesystem::exists(dbDir))
          throw "Database already exists!\n";

        return make_unique<impl>(dbName, dbDir);
      }

      static const unique_ptr<Idatabase> load(const string& dbName) {

        string dbDir = ".mdb/" + dbName;
        if (!filesystem::exists(dbDir))
          throw "Database not found!\n";
        return make_unique<impl>(dbName, dbDir);

      }

      void destroy() {
        memberKeyValueStore->clear();
      }

      string getDirectory() {
        return memberFullPath;
      }

      void setKeyValue(const string& key, const string& value) {
        memberKeyValueStore->setKeyValue(key, value);
      }

      string getKeyValue(const string& key) {
        return memberKeyValueStore->getKeyValue(key);
      }

    private:
      unique_ptr<keyValueStore> memberKeyValueStore;

      string memberName,
        memberFullPath;
    };
    unique_ptr<impl>memberImpl;

  public:

    databaseEmbedded(const string& dbName, const string& fullPath) : memberImpl(make_unique<impl>(dbName, fullPath)) {
      cout << "\n___" << dbName << '\t' << fullPath << "___\n";
    }

    databaseEmbedded(const string& dbname, const string& dbDirPath, unique_ptr<keyValueStore>& keyValueStore) : memberImpl(make_unique<impl>(dbname, dbDirPath, keyValueStore)) {}

    ~databaseEmbedded() {}

    void setKeyValue(const string& key, const string& value) {
      memberImpl->setKeyValue(key, value);
    }

    string getKeyValue(const string& key) {
      return memberImpl->getKeyValue(key);
    }

    static unique_ptr<Idatabase> createEmpty(const string& dbName) {
      return impl::createEmpty(dbName);
    }

    const static unique_ptr<Idatabase> load(const string& dbName) {
      return impl::load(dbName);
    }

    string getDirectory() {
      return memberImpl->getDirectory();
    }

    void destroy() {
      memberImpl->destroy();
    }
  };
}
#endif
