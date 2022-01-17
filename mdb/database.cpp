#include "database.h"
#include "string"
#include "iostream"
#include "filesystem"

namespace fs=std::filesystem;

database::database(const std::string&dbName, const std::string&fullPath): memberName(dbName), memberFullPath(fullPath)
{
    std::cout<<dbName<<" "<<fullPath;
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
