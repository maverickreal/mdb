#ifndef MDB_H
#define MDB_H
#include "string"
#include "database.h"

class Mdb
{
public:
    Mdb();
    static databaseEmbedded createFreshDB(const std::string& dbName);
    static databaseEmbedded loadDB(const std::string& dbName);
};

#endif
