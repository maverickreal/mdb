#include "cxxopts.h"
#include "mdb/database.h"
#include "mdb/mdb.h"

cxxopts::Options opts("mdb-cli", "CLI for MaverickDB");

#define logUsage() std::cout<<"Invalid configuration!\n";

int main(int argCnt, char*args[]){
    opts.add_options()
            ("c, create", "CREATE A DATABASE")
            ("d, destroy", "DESTROY A DATABASE")
            ("s, set","SET A KEY IN A DATABASE")
            ("g, get","GET A KEY FROM A DATABASE")
            ("n, name", "DATABASE NAME(MANDATORY)", cxxopts::value<std::string>())
            ("k, key", "KEY TO SET/GET",cxxopts::value<std::string>())
            ("v, value","VALUE TO SET", cxxopts::value<std::string>());
    auto res=opts.parse(argCnt, args);
    if(res.count("c")==1){
        if(!res.count("n")){
            std::cout<<"ERROR:\tMust provide database name with -n <name>\n";
            logUsage()
            return 1;
        }
        std::string dbName=res["n"].as<std::string>();
        database db(Mdb::createFreshDB(dbName));
        return 0;
    }
    if (res.count("s") == 1) {
        if (!res.count("n")) {
            std::cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }
        if (!res.count("k")) {
            std::cout << "ERROR:\tMust provide a key with -k <name>\n";
            logUsage();
            return 1;
        }
        if (!res.count("v")) {
            std::cout << "ERROR:\tMust provide a value with -v <value>\n";
            logUsage();
            return 1;
        }
        std::string dbName=res["n"].as<std::string>(),
                k(res["k"].as<std::string>()),
                v(res["v"].as<std::string>());
        database db(Mdb::loadDB(dbName));
        db.setKeyValue(k,v);
        return 0;
    }
    if(res.count("g")==1){
        if (!res.count("n")) {
            std::cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }
        if (!res.count("k")) {
            std::cout << "ERROR:\tMust provide a key with -k <name>\n";
            logUsage();
            return 1;
        }
        std::string dbName=res["n"].as<std::string>(),
                key=res["k"].as<std::string>();
        database db(Mdb::loadDB(dbName));
        std::cout<<db.getKeyValue(key)<<'\n';
        return 0;
    }
    if(res.count("d")==1){
        if(!res.count("n")) {
            std::cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }
        std::string dbName=res["n"].as<std::string>();
        database db(Mdb::loadDB(dbName));
        std::cout<<(db.destroy()?"Destroyed":"Failed")<<'\n';
        return 0;
    }
    std::cout<<"ERROR:\tNo command provided!\n";
    logUsage()
    return 1;
}
