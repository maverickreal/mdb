#ifndef DATABASE_H
#define DATABASE_H
#include "string"


class database
{
public:
    database(const std::string&dbName, const std::string&fullPath);
    static database createEmpty(const std::string&dbName);
};

#endif
