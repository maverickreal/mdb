#include "extensions/extension.h"
#include "string"
#include "filesystem"
#include "iostream"

using namespace ext;
using namespace std;
using namespace inc;

/* Healthy reminder -> https://www.cplusplus.com/reference/filesystemtream/ofilesystemtream/
 * https://www.cplusplus.com/reference/filesystemtream/ios/
 * ios::in allows input (read operations) from a stream
 * ios::out allows output (write operations) to a stream
 * ios::trunc similar to out, but strictly replaces the entire content
 * | (bitwise OR) for flags
 * https://www.cplusplus.com/reference/filesystemtream/ifilesystemtream/
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
  impl(const string& dbName, const string& fullPath);

  impl(const string& dbname, const string& fullpath, unique_ptr<keyValueStore>& keyValueStore);

  ~impl();

  string getDirectory();

  void setKeyValue(const string& key, const string& value);

  string getKeyValue(const string& key);

  static const unique_ptr<Idatabase> createEmpty(const string& dbName);

  static const unique_ptr<Idatabase> createEmpty(const string& dbname, unique_ptr<keyValueStore>& keyValueStore);

  unique_ptr<keyValueStore> memberKeyValueStore;

  static const unique_ptr<Idatabase> load(const string& dbName);

  void destroy();

private:

  string memberName,
    memberFullPath;
};

databaseEmbedded::impl::impl(const string& dbName, const string& dbDirPath) : memberName(dbName), memberFullPath(dbDirPath) {
  unique_ptr<keyValueStore> fileStore = make_unique<fileKeyValueStore>(dbDirPath),
    memoryStore = make_unique<memoryKeyValueStore>(fileStore);
  memberKeyValueStore = move(memoryStore);
}

databaseEmbedded::impl::impl(const string& dbname, const string& dbDirPath, unique_ptr<keyValueStore>& keyValueStore) :memberKeyValueStore(keyValueStore.release()) {
  memberName = dbname;
  memberFullPath = dbDirPath;
}

databaseEmbedded::impl :: ~impl() {}

const unique_ptr<Idatabase> databaseEmbedded::impl::createEmpty(const string& dbName) {
  const string baseDir(".mdb");

  if (!filesystem::exists(baseDir))
    filesystem::create_directory(baseDir);

  const string dbDir = baseDir + "/" + dbName;

  return make_unique<impl>(dbName, dbDir);
}

const unique_ptr<Idatabase> databaseEmbedded::impl::createEmpty(const string& dbname, unique_ptr<keyValueStore>& keyValueStore) {
  string basedir = ".mdb";
  if (!filesystem::exists(basedir))
    filesystem::create_directory(basedir);
  string dbfolder = basedir + "/" + dbname;
  return make_unique<databaseEmbedded::impl>(dbname, dbfolder, keyValueStore);
}

const unique_ptr<Idatabase> databaseEmbedded::impl::load(const string& dbName) {

  string baseDir(".mdb"),
    dbDir(baseDir + "/" + dbName);
  return make_unique<databaseEmbedded::impl>(dbName, dbDir);

}

void databaseEmbedded::impl::destroy() {
  memberKeyValueStore->clear();
}

string databaseEmbedded::impl::getDirectory() {
  return memberFullPath;
}

void databaseEmbedded::impl::setKeyValue(const string& key, const string& value) {
  memberKeyValueStore->setKeyValue(key, value);
}

string databaseEmbedded::impl::getKeyValue(const string& key) {
  return memberKeyValueStore->getKeyValue(key);
}

databaseEmbedded::databaseEmbedded(const string& dbName, const string& fullPath) : memberImpl(make_unique<databaseEmbedded::impl>(dbName, fullPath)) {
  cout << "\n___" << dbName << '\t' << fullPath << "___\n";
}

databaseEmbedded::databaseEmbedded(const string& dbname, const string& dbDirPath, unique_ptr<keyValueStore>& keyValueStore) : memberImpl(make_unique<databaseEmbedded::impl>(dbname, dbDirPath, keyValueStore)) {}

databaseEmbedded :: ~databaseEmbedded() {}

void databaseEmbedded::setKeyValue(const string& key, const string& value) {
  memberImpl->setKeyValue(key, value);
}

string databaseEmbedded::getKeyValue(const string& key) {
  return memberImpl->getKeyValue(key);
}

const unique_ptr<Idatabase> databaseEmbedded::createEmpty(const string& dbname, unique_ptr<keyValueStore>& keyValueStore) {
  return impl::createEmpty(dbname, keyValueStore);
}

unique_ptr<Idatabase> databaseEmbedded::createEmpty(const string& dbName) {
  return impl::createEmpty(dbName);
}

unique_ptr<Idatabase> databaseEmbedded::load(const string& dbName) {
  return impl::load(dbName);
}

string databaseEmbedded::getDirectory() {
  return memberImpl->getDirectory();
}

void databaseEmbedded::destroy() {
  memberImpl->destroy();
}
