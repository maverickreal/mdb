#ifndef MDB_H
#define MDB_H
#include<string>

class Mdb
{
public:
    Mdb();
    static database createFreshDB(const std::string&dbName);
};

#endif
