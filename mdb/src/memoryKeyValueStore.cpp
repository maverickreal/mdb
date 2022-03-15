#include "extensions/extension.h"
#include "unordered_map"
#include "optional"
#include "extensions/highwayhash.h"

using namespace inc;
using namespace ext;
using namespace std;

class memoryKeyValueStore::impl {
public:

  impl() {}

  impl(unique_ptr<keyValueStore>& toCache);

  unordered_map<string, string, highwayHash> memberKeyValueStore;

  optional<unique_ptr<keyValueStore>> memberCachedStore;
};

// Healthy Reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/release/
memoryKeyValueStore::impl::impl(unique_ptr<keyValueStore>& toCache) :memberCachedStore(toCache.release()) {}

memoryKeyValueStore::memoryKeyValueStore() : memberImpl(make_unique<memoryKeyValueStore::impl>()) {}

// Healthy Reminder - > https://www.cplusplus.com/reference/memory/unique_ptr/get/
memoryKeyValueStore::memoryKeyValueStore(unique_ptr<keyValueStore>& toCache) : memberImpl(make_unique<memoryKeyValueStore::impl>(toCache)) {
  memberImpl->memberCachedStore->get()->loadKeysInto([this](const string& key, const string& value) {
    memberImpl->memberKeyValueStore[key] = value;
    });
}

memoryKeyValueStore::~memoryKeyValueStore() {}

void memoryKeyValueStore::setKeyValue(const string& key, const string& value) {
  memberImpl->memberKeyValueStore[key] = value;
  if (memberImpl->memberCachedStore)
    memberImpl->memberCachedStore->get()->setKeyValue(key, value);
}

string memoryKeyValueStore::getKeyValue(const string& key) {
  const auto& v = memberImpl->memberKeyValueStore.find(key);
  return v == memberImpl->memberKeyValueStore.end() ? "" : v->second;
}

void memoryKeyValueStore::loadKeysInto(const function<void(string key, string value)>& callBack) {
  for (const auto& element : memberImpl->memberKeyValueStore)
    callBack(element.first, element.second);
}

void memoryKeyValueStore::clear() {
  memberImpl->memberKeyValueStore.clear();
  if (memberImpl->memberCachedStore)
    memberImpl->memberCachedStore->get()->clear();
}
