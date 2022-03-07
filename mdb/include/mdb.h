#ifndef MDB_H
#define MDB_H
#include "string"
#include "database.h"

using namespace Mdb;
namespace mdb {
    class Mdb
    {
    public:
        Mdb();
        static std::unique_ptr<Idatabase> createFreshDB(const std::string& dbName);
        static std::unique_ptr<Idatabase> loadDB(const std::string& dbName);
    };
}

#endif
