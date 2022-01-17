#include "mdb.h"
#include "database.h"
using namespace std;

/* Healthy reminders: https://www.educative.io/edpresso/what-is-the-using-keyword-in-cpp
                      https://www.geeksforgeeks.org/scope-resolution-operator-in-c */

Mdb::Mdb()
{
}

database Mdb::createFreshDB(const std::string&dbName, const std::string&fullPath){
    return database(dbName, fullPath);
}
