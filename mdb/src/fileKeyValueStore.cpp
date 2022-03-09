#include "extensions/extension.h"
#include "iostream"
#include "fstream"
#include "filesystem"

namespace mdbExt {

  namespace fs = std::filesystem;

  class fileKeyValueStore::impl {
  public:
    impl(const std::string& fullpath);
    std::string memberFullPath;
  };

  fileKeyValueStore::impl::impl(const std::string& fullpath) :memberFullPath(fullpath) {}

  fileKeyValueStore::fileKeyValueStore(const std::string& fullpath) :memberImpl(std::make_unique<fileKeyValueStore::impl>(fullpath)) {
    if (!fs::exists(fullpath))
      fs::create_directory(fullpath);
  }

  fileKeyValueStore::~fileKeyValueStore() {}

  void fileKeyValueStore::setKeyValue(const std::string& key, const std::string& value) {
    std::ofstream os;
    os.open(memberImpl->memberFullPath + "/" + key + "-string.kv", std::ios::out | std::ios::trunc);
    os << value;
    os.close();
  }

  std::string fileKeyValueStore::getKeyValue(const std::string& key) {
    std::ifstream t(memberImpl->memberFullPath + "/" + key + "-string.kv");
    std::string value;
    t.seekg(0, std::ios::end);
    value.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    value.assign((std::istreambuf_iterator<char>(t)),
      std::istreambuf_iterator<char>());
    return value;
  }


  void fileKeyValueStore::loadKeysInto(const std::function<void(std::string key, std::string value)>& callBack) {
    for (const auto& p : fs::directory_iterator(memberImpl->memberFullPath)) {
      if (p.exists() && p.is_regular_file()) {
        if (".kv" == p.path().extension()) {
          std::string key = p.path().filename();
          int cnt = 10;
          while (cnt-- && !key.empty())
            key.pop_back();
          std::ifstream t(p.path());
          std::string value;
          t.seekg(0, std::ios::end);
          value.reserve(t.tellg());
          t.seekg(0, std::ios::beg);
          value.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
          callBack(key, value);
        }
      }
    }
  }

  void fileKeyValueStore::clear() {
    if (fs::exists(memberImpl->memberFullPath))
      fs::remove_all(memberImpl->memberFullPath);
  }
}
