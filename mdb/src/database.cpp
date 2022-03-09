#include "database.h"
#include "extensions/extension.h"
#include "string"
#include "filesystem"
#include "iostream"

namespace fs = std::filesystem;
using namespace mdbExt;

/* Healthy reminder -> https://www.cplusplus.com/reference/fstream/ofstream/
 * https://www.cplusplus.com/reference/fstream/ios/
 * ios::in allows input (read operations) from a stream
 * ios::out allows output (write operations) to a stream
 * ios::trunc similar to out, but strictly replaces the entire content
 * | (bitwise OR) for flags
 * https://www.cplusplus.com/reference/fstream/ifstream/
 * https://www.cplusplus.com/reference/istream/istream/tellg/
 * https://www.cplusplus.com/reference/istream/istream/seekg/
 * https://www.cplusplus.com/reference/iterator/istreambuf_iterator/
 * A stream is a FIFO data structure. A buffer is random access.
 * A buffer is an array which contains some (raw) data, a stream is an abstraction of a buffer ( each stream has a buffer ).
 * A buffer is a region of memory used to temporarily hold data while it is being moved from one place to another.
 * A stream is a source or sink of data, usually individual bytes or characters.
 * ios and istreambuf_iterator operate upon stream and buffer resp.
*/

class databaseEmbedded::impl : public Idatabase {
public:
  impl(const std::string& dbName, const std::string& fullPath);

  impl(const std::string& dbname, const std::string& fullpath, std::unique_ptr<keyValueStore>& keyValueStore);

  ~impl();

  std::string getDirectory();

  void setKeyValue(const std::string& key, const std::string& value);

  std::string getKeyValue(const std::string& key);

  static const std::unique_ptr<Idatabase> createEmpty(const std::string& dbName);

  static const std::unique_ptr<Idatabase> createEmpty(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore);

  std::unique_ptr<keyValueStore> memberKeyValueStore;

  static const std::unique_ptr<Idatabase> load(const std::string& dbName);

  void destroy();

private:

  std::string memberName,
    memberFullPath;
};

databaseEmbedded::impl::impl(const std::string& dbName, const std::string& dbDirPath) : memberName(dbName), memberFullPath(dbDirPath) {
  std::unique_ptr<keyValueStore> fileStore = std::make_unique<fileKeyValueStore>(dbDirPath),
    memoryStore = std::make_unique<memoryKeyValueStore>(fileStore);
  memberKeyValueStore = std::move(memoryStore);
}

databaseEmbedded::impl::impl(const std::string& dbname, const std::string& dbDirPath, std::unique_ptr<keyValueStore>& keyValueStore) :memberKeyValueStore(keyValueStore.release()) {
  memberName = dbname;
  memberFullPath = dbDirPath;
}

databaseEmbedded::impl :: ~impl() {}

const std::unique_ptr<Idatabase> databaseEmbedded::impl::createEmpty(const std::string& dbName) {
  const std::string baseDir(".mdb");

  if (!fs::exists(baseDir))
    fs::create_directory(baseDir);

  const std::string dbDir = baseDir + "/" + dbName;

  return std::make_unique<impl>(dbName, dbDir);
}

const std::unique_ptr<Idatabase> databaseEmbedded::impl::createEmpty(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore) {
  std::string basedir = ".mdb";
  if (!fs::exists(basedir))
    fs::create_directory(basedir);
  std::string dbfolder = basedir + "/" + dbname;
  return std::make_unique<databaseEmbedded::impl>(dbname, dbfolder, keyValueStore);
}

const std::unique_ptr<Idatabase> databaseEmbedded::impl::load(const std::string& dbName) {

  std::string baseDir(".mdb"),
    dbDir(baseDir + "/" + dbName);
  return std::make_unique<databaseEmbedded::impl>(dbName, dbDir);

}

void databaseEmbedded::impl::destroy() {
  memberKeyValueStore->clear();
}

std::string databaseEmbedded::impl::getDirectory() {
  return memberFullPath;
}

void databaseEmbedded::impl::setKeyValue(const std::string& key, const std::string& value) {
  memberKeyValueStore->setKeyValue(key, value);
}

std::string databaseEmbedded::impl::getKeyValue(const std::string& key) {
  return memberKeyValueStore->getKeyValue(key);
}

databaseEmbedded::databaseEmbedded(const std::string& dbName, const std::string& fullPath) : memberImpl(std::make_unique<databaseEmbedded::impl>(dbName, fullPath)) {
  std::cout << "\n___" << dbName << '\t' << fullPath << "___\n";
}

databaseEmbedded::databaseEmbedded(const std::string& dbname, const std::string& dbDirPath, std::unique_ptr<keyValueStore>& keyValueStore) :memberImpl(std::make_unique<databaseEmbedded::impl>(dbname, dbDirPath, keyValueStore)) {}

databaseEmbedded :: ~databaseEmbedded() {}

void databaseEmbedded::setKeyValue(const std::string& key, const std::string& value) {
  memberImpl->setKeyValue(key, value);
}

std::string databaseEmbedded::getKeyValue(const std::string& key) {
  return memberImpl->getKeyValue(key);
}

const std::unique_ptr<Idatabase> databaseEmbedded::createEmpty(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore) {
  return databaseEmbedded::impl::createEmpty(dbname, keyValueStore);
}

std::unique_ptr<Idatabase> databaseEmbedded::createEmpty(const std::string& dbName) {
  return impl::createEmpty(dbName);
}

std::unique_ptr<Idatabase> databaseEmbedded::load(const std::string& dbName) {
  return impl::load(dbName);
}

std::string databaseEmbedded::getDirectory() {
  return memberImpl->getDirectory();
}

void databaseEmbedded::destroy() {
  memberImpl->destroy();
}
