#ifndef MDB_H
#define MDB_H

#include "string"
#include "database.h"

namespace inc {
    class Mdb
    {
    public:

        static std::unique_ptr<Idatabase> createFreshDB(const std::string& dbName);

        static std::unique_ptr<Idatabase> createFreshDB(const std::string& dbname, std::unique_ptr<keyValueStore>& keyValueStore);

        static std::unique_ptr<Idatabase> loadDB(const std::string& dbName);
    };
}
#endif