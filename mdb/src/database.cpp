#include "database.h"
#include "string"
#include "iostream"
#include "fstream"
#include "filesystem"

namespace fs = std::filesystem;

class databaseEmbedded : public Idatabase{
public:
    databaseEmbedded(const std::string&dbName, const std::string&fullPath);

    virtual ~databaseEmbedded()=0;

    virtual bool destroy()=0;

    virtual void setKeyValue(const std::string& key, const std::string& value)=0;

    virtual std::string getKeyValue(const std::string& key)=0;

    static Idatabase& createEmpty(const std::string& dbName);

    static Idatabase& load(const std::string& dbName);

    virtual std::string getDirectory(void)=0;
private:
    class impl;
    // Healthy reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/
    std::unique_ptr<impl>memberImpl;
};

class databaseEmbedded :: impl : public Idatabase{
//public:
    impl(const std::string&dbName, const std::string&fullPath);

    virtual ~impl()=0;

    virtual bool destroy()=0;

    virtual void setKeyValue(const std::string& key, const std::string& value)=0;

    virtual std::string getKeyValue(const std::string& key)=0;

    static Idatabase& createEmpty(const std::string& dbName);

    static Idatabase& load(const std::string& dbName);

    virtual std::string getDirectory(void)=0;

private:
    std::string memberName, memberFullPath;
};

databaseEmbedded :: impl :: impl(const std::string&dbName, const std::string&fullPath) : memberName(dbName), memberFullPath(fullPath){;}

/* Healthy reminder -> https://www.cplusplus.com/reference/fstream/ofstream/
 * https://www.cplusplus.com/reference/fstream/ios/
 * ios::in allows input (read operations) from a stream
 * ios::out allows output (write operations) to a stream
 * ios::trunc similar to out, but strictly replaces the entire content
 * | (bitwise OR) for flags
*/

databaseEmbedded::databaseEmbedded(const std::string& dbName, const std::string& fullPath) : memberImpl(std::make_unique<databaseEmbedded::impl>(dbName, fullPath))
{
    std::cout << "\n___" << dbName << '\t' << fullPath << "___\n";
}

void databaseEmbedded::impl::setKeyValue(const std::string& key, const std::string& value) {
    std::ofstream ofs(memberFullPath + "/" + key + "-string.kv", std::ios::out | std::ios::trunc);
    //performs the write operation
    ofs << value;
    ofs.close();
}

/* Healthy reminder -> https://www.cplusplus.com/reference/fstream/ifstream/
 * https://www.cplusplus.com/reference/istream/istream/tellg/
 * https://www.cplusplus.com/reference/istream/istream/seekg/
 * https://www.cplusplus.com/reference/iterator/istreambuf_iterator/
 * A stream is a FIFO data structure. A buffer is random access.
 * A buffer is an array which contains some (raw) data, a stream is an abstraction of a buffer ( each stream has a buffer ).
 * A buffer is a region of memory used to temporarily hold data while it is being moved from one place to another.
 * A stream is a source or sink of data, usually individual bytes or characters.
 * ios and istreambuf_iterator operate upon stream and buffer resp.
*/
std::string databaseEmbedded::impl::getKeyValue(const std::string& key) {
    std::ifstream ifs(memberFullPath + "/" + key + "-string.kv");
    std::string value;
    ifs.seekg(0, std::ios::end);
    value.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    value.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    return value;
}

Idatabase& databaseEmbedded::impl::createEmpty(const std::string& dbName) {
    const std::string baseDir = ".mdb";
    if (!fs::exists(baseDir))
        fs::create_directory(baseDir);
    const std::string dbDir = baseDir + "/" + dbName;
    if (!fs::exists(dbDir))
        fs::create_directory(dbDir);
    auto res=databaseEmbedded::impl(dbName, dbDir);
    return res;

}

Idatabase& databaseEmbedded::impl::load(const std::string& dbName) {
    const std::string baseDir = ".mdb",
        dbDir = baseDir + "/" + dbName;
    auto res=databaseEmbedded::impl(dbName, dbDir);
    return res;
}

std::string databaseEmbedded::impl::getDirectory() {
    return memberFullPath;
}

bool databaseEmbedded::impl::destroy() {
    if (fs::exists(memberFullPath)) {
        // delete the dir
        fs::remove_all(memberFullPath);
        return true;
    }
    return false;
}
