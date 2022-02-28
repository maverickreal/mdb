#include "database.h"
#include "extensions/extension.h"
#include "string"
#include "iostream"
#include "fstream"
#include "filesystem"

namespace fs = std :: filesystem;
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

class databaseEmbedded :: impl : public Idatabase {
public:
  impl(const std :: string&dbName, const std :: string&fullPath);

  ~impl();

  std :: string getDirectory();

  void setKeyValue(const std :: string&key, const std :: string&value);

  std :: string getKeyValue(const std :: string&key);

  static const std :: unique_ptr<Idatabase> createEmpty(const std :: string&dbName);

  static const std :: unique_ptr<Idatabase> load(const std :: string&dbName);

  bool destroy();

private:

  std :: string memberName;
  std :: string memberFullPath;

};

databaseEmbedded :: impl :: impl(const std :: string&dbName, const std :: string&dbDirPath) : memberName(dbName), memberFullPath(dbDirPath){
    std :: cout<<"\nPrinting memberFullPath\t:\t"<<memberFullPath;
}

databaseEmbedded :: impl :: ~impl() {;}

const std :: unique_ptr<Idatabase> databaseEmbedded :: impl :: createEmpty(const std :: string&dbName) {
  const std :: string baseDir(".mdb");

  if (!fs :: exists(baseDir))
      fs :: create_directory(baseDir);

  const std :: string dbDir = baseDir + "/" + dbName;

  if (!fs :: exists(dbDir))
      fs :: create_directory(dbDir);

  return std :: make_unique<impl>(dbName, dbDir);
}

const std :: unique_ptr<Idatabase> databaseEmbedded :: impl :: load(const std :: string&dbName) {

  std :: string baseDir(".mdb"),
          dbDir(baseDir + "/" + dbName);
  return std :: make_unique<databaseEmbedded :: impl>(dbName, dbDir);

}

bool databaseEmbedded :: impl :: destroy() {
  if (fs::exists(memberFullPath)){
      fs::remove_all(memberFullPath);
      return true;
  }
  return false;
}

std :: string databaseEmbedded :: impl :: getDirectory() {
  return memberFullPath;
}

void databaseEmbedded :: impl :: setKeyValue(const std :: string&key, const std :: string&value) {

  std :: ofstream ofs;
  ofs.open(memberFullPath + "/" + key + "-string.kv", std :: ios :: out | std :: ios :: trunc);
  ofs << value;
  ofs.close();

}

std :: string databaseEmbedded :: impl :: getKeyValue(const std :: string&key) {

  std :: ifstream t(memberFullPath + "/" + key + "-string.kv");
  std :: string value;
  t >> value;
  return value;

}

databaseEmbedded :: databaseEmbedded(const std :: string& dbName, const std :: string& fullPath) : memberImpl(std :: make_unique<databaseEmbedded :: impl>(dbName, fullPath)){
    std :: cout << "\n___" << dbName << '\t' << fullPath << "___\n";
}

databaseEmbedded :: ~databaseEmbedded(){}

void databaseEmbedded :: setKeyValue(const std :: string& key, const std :: string& value) {
    memberImpl->setKeyValue(key, value);
}

std :: string databaseEmbedded :: getKeyValue(const std :: string& key) {
    return memberImpl->getKeyValue(key);
}

std :: unique_ptr<Idatabase> databaseEmbedded :: createEmpty(const std :: string& dbName) {
    return impl :: createEmpty(dbName);
}

std :: unique_ptr<Idatabase> databaseEmbedded :: load(const std :: string& dbName) {
    return impl :: load(dbName);
}

std :: string databaseEmbedded :: getDirectory() {
    return memberImpl->getDirectory();
}

bool databaseEmbedded :: destroy() {
    return memberImpl->destroy();
}
