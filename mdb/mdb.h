#ifndef MDB_H
#define MDB_H
#include "string"
#include "database.h"

class Mdb
{
public:
    Mdb();
    static database createFreshDB(const std::string&dbName);
    static database loadDB(const std::string&dbName);
};

#endif
