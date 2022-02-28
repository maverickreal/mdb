#ifndef DATABASE_H
#define DATABASE_H
#include "string"
#include "memory"

namespace Mdb{

    class Idatabase{
    public:
        // Healthy reminder -> https://www.geeksforgeeks.org/virtual-function-cpp/
        //                     https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/
        //                     https://www.geeksforgeeks.org/explicitly-defaulted-deleted-functions-c-11/
        Idatabase()=default;

        virtual ~Idatabase()=default;

        virtual bool destroy()=0;

        virtual void setKeyValue(const std :: string& key, const std :: string& value)=0;

        virtual std :: string getKeyValue(const std :: string& key)=0;

        static const std :: unique_ptr<Idatabase>createEmpty(const std :: string& dbName);

        static const std :: unique_ptr<Idatabase>load(const std :: string& dbName);

        virtual std :: string getDirectory(void)=0;// takes no args

        // An instruction to the compiler to not privately create a constructor
    };

}
#endif
