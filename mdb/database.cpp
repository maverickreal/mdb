#include "database.h"
#include "string"

database::database(const std::string&dbName, const std::string&fullPath)
{

}

database database::createEmpty(const std::string&dbName){
    return database(dbName, "fullpath");
}
