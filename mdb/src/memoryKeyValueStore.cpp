#include "extensions/extension.h"
#include "unordered_map"
#include"optional"

namespace mdbExt {

  class memoryKeyValueStore::impl {
  public:
    impl() {}
    impl(std::unique_ptr<keyValueStore>& toCache);
    std::unordered_map<std::string, std::string> memberKeyValueStore;
    std::optional<std::unique_ptr<keyValueStore>> memberCachedStore;
  };

  // Healthy Reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/release/
  memoryKeyValueStore::impl::impl(std::unique_ptr<keyValueStore>& toCache) :memberCachedStore(toCache.release()) {}

  memoryKeyValueStore::memoryKeyValueStore() :memberImpl(std::make_unique<memoryKeyValueStore::impl>()) {}

  // Healthy Reminder - > https://www.cplusplus.com/reference/memory/unique_ptr/get/
  memoryKeyValueStore::memoryKeyValueStore(std::unique_ptr<keyValueStore>& toCache) :memberImpl(std::make_unique<memoryKeyValueStore::impl>(toCache)) {
    memberImpl->memberCachedStore->get()->loadKeysInto([this](const std::string& key, const std::string& value) {
      memberImpl->memberKeyValueStore[key] = value;
      });
  }


  memoryKeyValueStore::~memoryKeyValueStore() {}

  void memoryKeyValueStore::setKeyValue(const std::string& key, const std::string& value) {
    memberImpl->memberKeyValueStore[key] = value;
    if (memberImpl->memberCachedStore)
      memberImpl->memberCachedStore->get()->setKeyValue(key, value);
  }

  std::string memoryKeyValueStore::getKeyValue(const std::string& key) {
    const auto& v = memberImpl->memberKeyValueStore.find(key);
    return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
  }

  void memoryKeyValueStore::loadKeysInto(const std::function<void(std::string key, std::string value)>& callBack) {
    for (const auto& element : memberImpl->memberKeyValueStore)
      callBack(element.first, element.second);
  }

  void memoryKeyValueStore::clear() {
    memberImpl->memberKeyValueStore.clear();
    if (memberImpl->memberCachedStore)
      memberImpl->memberCachedStore->get()->clear();
  }

}
