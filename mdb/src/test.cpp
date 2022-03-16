#include "iostream"
#include "../include/mdb.h"

using namespace std;

int main() {
    string dbname;
    cout << "Enter database name:\n";
    cin >> dbname;
    unique_ptr<Idatabase>db;
    try {
        db = Mdb::createFreshDB(dbname);
        cout << "Database created successfully.\n";
    }
    catch (...) {
        db = Mdb::loadDB(dbname);
        cout << "Database loaded successfully.\n";
    }
    string key, value;
    cout << "Enter key:\n";
    cin >> key;
    cout << "Enter value:\n";
    cin >> value;
    db->setKeyValue(key, value);
    cout << "Value set successfully.\n";
    cout << "Value of key " << key << " is " << db->getKeyValue(key) << '\n';
    cout << "Delete database? (y/n)\n";
    char ch;
    cin >> ch;
    if (ch == 'y') {
        db->destroy();
        cout << "Database deleted successfully.\n";
    }
    return 0;
}