#include "iostream"
#include "string"
#include "../include/mdb.h"
#include "../include/admin.h"
#include "../include/user.h"

using namespace std;
using namespace mdb;
using namespace admin;
using namespace user;

int main() {
    string userName;
    cout << "Enter user name:\n";
    cin >> userName;
    string password;
    cout << "Enter password:\n";
    cin >> password;
    unique_ptr<IUser>user;
    bool createdUser = true;
    try {
        cout << "Loading user...\n";
        user = loadUser(userName, password);
        cout << "User loaded successfully!\n";
        createdUser = false;
    }
    catch (...) {
        cout << "User does'nt exist!\nCreating user...\n";
        user = createFreshUser(userName, password);
        cout << "User created successfully!\n";
    }
    if (createdUser) {
        cout << "Make this user admin? (y/n)\n";
        char c;
        cin >> c;
        if (c == 'y') {
            createAdmin(userName, password);
            cout << "User is now admin!\n";
        }
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
    int  mx;
    cout << "Enter the size of dataset for testing purpose:\n";
    cin >> mx;
    for (int i = 0; i < mx; i++)
        v.push_back(to_string(i));
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (const string& it : v)
        db->setKeyValue(it, it);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time taken for " << mx << " setKeyValue calls: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms\n";
    begin = chrono::steady_clock::now();
    for (const string& it : v)
        db->getKeyValue(it);
    end = chrono::steady_clock::now();
    cout << "Time taken for " << mx << " getKeyValue calls: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms\n";
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