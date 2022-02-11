#ifndef extension_H
#define extension_h
#include "database.h"
namespace mdbExt{
    class databaseEmbedded : public Idatabase{
    public:
        databaseEmbedded(const std :: string&dbName, const std :: string&fullPath);

        ~databaseEmbedded();

        bool destroy();

        void setKeyValue(const std :: string& key, const std :: string& value);

        std :: string getKeyValue(const std :: string& key);

        static std :: unique_ptr<Idatabase> createEmpty(const std :: string& dbName);

        static std :: unique_ptr<Idatabase> load(const std :: string& dbName);

        std :: string getDirectory(void);

        class impl;
    private:
        // Healthy reminder -> https://www.cplusplus.com/reference/memory/unique_ptr/
        std :: unique_ptr<impl>memberImpl;
    };
}
#endif
