#ifndef STORES_H
#define STORES_H

#include "database.h"
#include "filesystem"
#include "functional"
#include "fstream"
#include "iostream"
#include "optional"
#include "hash/hash.h"
#include "unordered_map"
#include "list"

#define endl '\n';

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
using namespace customHash;

namespace inc {

#define rootDir "._mdb/"
#define userDB "._users/"
#define internal_user "._system/"

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

    void setKeyValue(const Date& key, const string& value) {
      memberImpl->memberKeyValueStore[key.getDate()] = value;
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->setKeyValue(key.getDate(), value);
    }

    void setKeyValue(const double& key, const string& value) {
      memberImpl->memberKeyValueStore[to_string(key)] = value;
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->setKeyValue(to_string(key), value);
    }

    string getKeyValue(const string& key) {
      const auto& v = memberImpl->memberKeyValueStore.find(key);
      return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
    }

    string getKeyValue(const Date& key) {
      const auto& v = memberImpl->memberKeyValueStore.find(key.getDate());
      return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
    }

    string getKeyValue(const double& key) {
      const auto& v = memberImpl->memberKeyValueStore.find(to_string(key));
      return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
    }

    bool keyExists(const string& key) {
      return memberImpl->memberKeyValueStore.find(key) != memberImpl->memberKeyValueStore.end();
    }

    bool keyExists(const Date& key) {
      return memberImpl->memberKeyValueStore.find(key.getDate()) != memberImpl->memberKeyValueStore.end();
    }

    bool keyExists(const double& key) {
      return memberImpl->memberKeyValueStore.find(to_string(key)) != memberImpl->memberKeyValueStore.end();
    }

    void removeKeyValue(const string& key) {
      memberImpl->memberKeyValueStore.erase(key);
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->removeKeyValue(key);
    }

    void removeKeyValue(const Date& key) {
      memberImpl->memberKeyValueStore.erase(key.getDate());
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->removeKeyValue(key.getDate());
    }

    void removeKeyValue(const double& key) {
      memberImpl->memberKeyValueStore.erase(to_string(key));
      if (memberImpl->memberCachedStore)
        memberImpl->memberCachedStore->get()->removeKeyValue(to_string(key));
    }

    void loadKeysInto(const function<void(const string& key, const string& value)>& callBack) {
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

      unordered_map<string, string, xxHash> memberKeyValueStore;

      optional<unique_ptr<keyValueStore>> memberCachedStore;
    };

    unique_ptr<impl> memberImpl;
  };

  class fileKeyValueStore : public keyValueStore {
  public:

    fileKeyValueStore(const string& fullPath) : memberImpl(make_unique<fileKeyValueStore::impl>(fullPath)) {
      if (!filesystem::exists(fullPath))
        filesystem::create_directory(fullPath);
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

    void removeKeyValue(const string& key) {
      filesystem::remove(memberImpl->memberFullPath + "/" + key + "-string.kv");
    }

    void loadKeysInto(const function<void(const string& key, const string& value)>& callBack) {
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

    bool keyExists(const string& key) {
      return filesystem::exists(memberImpl->memberFullPath + "/" + key + "-string.kv");
    }

    void clear() {
      try {
        if (filesystem::exists(memberImpl->memberFullPath))
          filesystem::remove_all(memberImpl->memberFullPath);
      }
      catch (filesystem::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
      }
    }

  private:
    class impl {
    public:
      string memberFullPath;
      impl(const string& fullPath) :memberFullPath(move(fullPath)) {}
    };

    unique_ptr<impl> memberImpl;
  };

  class databaseEmbedded : public Idatabase {
  private:
    class impl : public Idatabase {
    public:

      impl(const string& dbName, const string& dbDirPath) : memberName(dbName), memberFullPath(dbDirPath) {
        unique_ptr<keyValueStore> fileStore = make_unique<fileKeyValueStore>(dbDirPath);
        memberKeyValueStore = make_unique<memoryKeyValueStore>(fileStore);
      }

      impl(const string& dbName, const string& dbDirPath, unique_ptr<keyValueStore>& keyValueStore) :memberKeyValueStore(keyValueStore.release()) {
        memberName = dbName;
        memberFullPath = dbDirPath;
      }

      ~impl() {}

      static const unique_ptr<Idatabase> createEmpty(const string& dbName, const string& userName) {

        string path = rootDir + userName;
        if (path.back() != '/')
          path.push_back('/');

        if (!filesystem::exists(path))
          filesystem::create_directory(path);

        const string dbDir = path + dbName;

        if (filesystem::exists(dbDir))
          throw "Database already exists!\n";

        return make_unique<impl>(dbName, dbDir);
      }

      static const unique_ptr<Idatabase> load(const string& dbName, const string& userName) {

        string dbDir = rootDir + userName + "/" + dbName;
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

      list<pair<string, string>>& getKeysAndValues() {
        list<pair<string, string>> result;
        memberKeyValueStore->loadKeysInto([&result](const string& key, const string& value) {
          result.push_back(make_pair(key, value));
          });
        return result;
      }

      void setKeyValue(const string& key, const string& value) {
        memberKeyValueStore->setKeyValue(key, value);
      }

      void setKeyValue(const Date& key, const string& value) {
        memberKeyValueStore->setKeyValue(key, value);
      }

      void setKeyValue(const double& key, const string& value) {
        memberKeyValueStore->setKeyValue(key, value);
      }

      string getKeyValue(const string& key) {
        return memberKeyValueStore->getKeyValue(key);
      }

      string getKeyValue(const Date& key) {
        return memberKeyValueStore->getKeyValue(key);
      }

      string getKeyValue(const double& key) {
        return memberKeyValueStore->getKeyValue(key);
      }

      bool keyExists(const string& key) {
        return memberKeyValueStore->keyExists(key);
      }

      bool keyExists(const Date& key) {
        return memberKeyValueStore->keyExists(key);
      }

      bool keyExists(const double& key) {
        return memberKeyValueStore->keyExists(key);
      }

      void removeKeyValue(const string& key) {
        memberKeyValueStore->removeKeyValue(key);
      }

      void removeKeyValue(const Date& key) {
        memberKeyValueStore->removeKeyValue(key);
      }

      void removeKeyValue(const double& key) {
        memberKeyValueStore->removeKeyValue(key);
      }

    private:
      unique_ptr<keyValueStore> memberKeyValueStore;

      string memberName,
        memberFullPath;
    };
    unique_ptr<impl>memberImpl;

  public:

    void setKeyValue(const string& key, const string& value) {
      memberImpl->setKeyValue(key, value);
    }

    void setKeyValue(const Date& key, const string& value) {
      memberImpl->setKeyValue(key, value);
    }

    void setKeyValue(const double& key, const string& value) {
      memberImpl->setKeyValue(key, value);
    }

    string getKeyValue(const string& key) {
      return memberImpl->getKeyValue(key);
    }

    string getKeyValue(const Date& key) {
      return memberImpl->getKeyValue(key);
    }

    string getKeyValue(const double& key) {
      return memberImpl->getKeyValue(key);
    }

    bool keyExists(const string& key) {
      return memberImpl->keyExists(key);
    }

    bool keyExists(const Date& key) {
      return memberImpl->keyExists(key);
    }

    bool keyExists(const double& key) {
      return memberImpl->keyExists(key);
    }

    void removeKeyValue(const string& key) {
      memberImpl->removeKeyValue(key);
    }

    void removeKeyValue(const Date& key) {
      memberImpl->removeKeyValue(key);
    }

    void removeKeyValue(const double& key) {
      memberImpl->removeKeyValue(key);
    }

    static unique_ptr<Idatabase> createEmpty(const string& dbName, const string& userName) {
      return impl::createEmpty(dbName, userName);
    }

    static unique_ptr<Idatabase> load(const string& dbName, const string& userName) {
      return impl::load(dbName, userName);
    }

    string getDirectory() {
      return memberImpl->getDirectory();
    }

    list<pair<string, string>>& getKeysAndValues() {
      return memberImpl->getKeysAndValues();
    }

    void destroy() {
      memberImpl->destroy();
    }
  };
}
#endif