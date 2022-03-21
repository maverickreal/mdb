#include "iostream"
#include "../include/mdb.h"

using namespace std;
using namespace inc;

int main() {
    string userName;
    cout << "Enter user name:\n";
    cin >> userName;
    string password;
    cout << "Enter password:\n";
    cin >> password;
    unique_ptr<IUser>user;
    try {
        user = Mdb::loadUser(userName, password);
        cout << "User loaded successfully!\n";
    }
    catch (...) {
        user = Mdb::createFreshUser(userName, password);
        cout << "User created successfully!\n";
    }
    string dbname;
    cout << "Enter database name:\n";
    cin >> dbname;
    unique_ptr<Idatabase>db;
    try {
        db = user->createFreshDB(dbname);
        cout << "Database created successfully.\n";
    }
    catch (...) {
        db = user->loadDB(dbname);
        cout << "Database loaded successfully.\n";
    }
    vector<string>v;
    long  mx = 100000;
    for (int i = 0; i < mx; i++)
        v.push_back(to_string(i));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (auto it = v.begin(); it != v.end(); it++, db->setKeyValue(*it, *it));
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time taken for " << mx << " setKeyValue calls: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
    begin = std::chrono::steady_clock::now();
    for (auto it = v.begin(); it != v.end(); it++)
        auto x = db->getKeyValue(*it);
    end = std::chrono::steady_clock::now();
    std::cout << "Time taken for " << mx << " getKeyValue calls: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
    cout << "Delete database? (y/n)\n";
    char ch;
    cin >> ch;
    if (ch == 'y') {
        db->destroy();
        cout << "Database deleted successfully.\n";
    }
    cout << "Delete user? (y/n)\n";
    cin >> ch;
    if (ch == 'y') {
        user->destroy();
        cout << "User deleted successfully.\n";
    }
    return 0;
}