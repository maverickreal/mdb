#include "database.h"
#include "string"
#include "iostream"

database::database(const std::string&dbName, const std::string&fullPath)
{
    std::cout<<dbName<<" "<<fullPath;
}

database database::createEmpty(const std::string&dbName){
    return database(dbName, "fullpath");
}


std::string database::getDirectory(){
    return "";
}
