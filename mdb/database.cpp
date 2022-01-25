#include "database.h"
#include "string"
#include "iostream"
#include "fstream"
#include "filesystem"

namespace fs=std::filesystem;

database::database(const std::string&dbName, const std::string&fullPath): memberName(dbName), memberFullPath(fullPath)
{
    std::cout<<dbName<<'\t'<<fullPath<<'\n';
}

/* Healthy reminder -> https://www.cplusplus.com/reference/fstream/ofstream/
 * https://www.cplusplus.com/reference/fstream/ios/
 * ios::in allows input (read operations) from a stream
 * ios::out allows output (write operations) to a stream
 * ios::trunc similar to out, but strictly replaces the entire content
 * | (bitwise OR) for flags
*/
void database::setKeyValue(const std::string&key, const std::string&value){
    std::ofstream ofs(memberFullPath+"/"+key+"-string.kv", std::ios::out | std::ios::trunc);
    //performs the write operation
    ofs<<value;
    ofs.close();
}

/* Healthy reminder -> https://www.cplusplus.com/reference/fstream/ifstream/
 * https://www.cplusplus.com/reference/istream/istream/tellg/
 * https://www.cplusplus.com/reference/istream/istream/seekg/
*/
std::string getKeyValue(const std::string&key){
    std::ifstream ifs(memberFullPath"/"+key+"-string.kv");
    std::string value;
    ifs.seekg(0,std::ios::end);
    value.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::end);
    return "";
}

std::string database::getKeyValue(const std::string&key){
    return "sigma";
}

database database::createEmpty(const std::string&dbName){
    const std::string baseDir=".mdb";
    if(!fs::exists(baseDir))
        fs::create_directory(baseDir);
    const std::string dbDir=baseDir+"/"+dbName;
    if(!fs::exists(dbDir))
        fs::create_directory(dbDir);
    return database(dbName, dbDir);

}

std::string database::getDirectory(){
    return memberFullPath;
}
