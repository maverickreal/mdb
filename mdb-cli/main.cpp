#include "cxxopts.h"
#include "../mdb/include/mdb.h"

using namespace mdb;
using namespace std;

cxxopts::Options opts("mdb-cli", "CLI for MaverickDB");

#define logUsage() std :: cout<<"Invalid configuration!\n";

int main(int argCnt, char* args[]) {

    opts.add_options()
        ("c, create", "CREATE A DATABASE")
        ("d, destroy", "DESTROY A DATABASE")
        ("s, set", "SET A KEY IN A DATABASE")
        ("g, get", "GET A KEY FROM A DATABASE")
        ("n, name", "DATABASE NAME(MANDATORY)", cxxopts::value<string>())
        ("k, key", "KEY TO SET/GET", cxxopts::value<string>())
        ("v, value", "VALUE TO SET", cxxopts::value<string>());

    auto res = opts.parse(argCnt, args);

    if (res.count("c") == 1) {

        if (!res.count("n")) {
            cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage()
                return 1;
        }

        string dbName = res["n"].as<string>();
        createFreshDB(dbName);
        return 0;
    }

    if (res.count("s") == 1) {
        if (!res.count("n")) {
            cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }

        if (!res.count("k")) {
            cout << "ERROR:\tMust provide a key with -k <name>\n";
            logUsage();
            return 1;
        }

        if (!res.count("v")) {
            cout << "ERROR:\tMust provide a value with -v <value>\n";
            logUsage();
            return 1;
        }

        string dbName = res["n"].as<string>(),
            k(res["k"].as<string>()),
            v(res["v"].as<string>());

        loadDB(dbName)->setKeyValue(k, v);
        return 0;
    }

    if (res.count("g") == 1) {
        if (!res.count("n")) {
            cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }

        if (!res.count("k")) {
            cout << "ERROR:\tMust provide a key with -k <name>\n";
            logUsage();
            return 1;
        }

        string dbName = res["n"].as<string>(),
            key = res["k"].as<string>();
        cout << loadDB(dbName)->getKeyValue(key) << '\n';
        return 0;
    }

    if (res.count("d") == 1) {
        if (!res.count("n")) {
            cout << "ERROR:\tMust provide database name with -n <name>\n";
            logUsage();
            return 1;
        }

        string dbName = res["n"].as<string>();
        loadDB(dbName)->destroy();
        return 0;
    }

    cout << "ERROR:\tNo command provided!\n";
    logUsage()
        return 1;
}
