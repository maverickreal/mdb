/* #define CATCH_CONFIG_ENABLE_BENCHMARKING 1
#include "catch.hpp"
#include "unordered_map"
#include "iostream"
#include "chrono"
#include "string"
#include "types.h"
#include "hashes.h"
#include "../mdb/mdb.h"
#include "../mdb/include/extensions/extension.h"
#define endl '\n';

using namespace mdb;
using namespace std;

TEST_CASE("Measure basic performance", "[!hide][performance][setKeyValue][getKeyValue]") {
    SECTION("Store and Retrieve 100,000 keys. (Memory cached key-value store)") {
        cout << "_-_-_-_-_-_-Default key-value store performance test-_-_-_-_-_-_" << endl;
        string dbName = "myemptydb";
        unique_ptr<Idatabase> db(createFreshDB(dbName));

        int total = 100000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        for (; i < total;i++)
            keyValues.push_back({ hashedKey(to_string(i)), encodedValue(to_string(i)) });

        cout << "Key size is max " << to_string(total - 1).length() << " bytes" << endl;

        long every = 1000;
        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        cout << "====== SET ======" << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, move(it->second));
            ++i;
            if (0 == i % every)
                cout << ".";
        }
        cout << endl;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second" << endl;
        cout << endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        string aString = "blank";
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++)
            result = db->getKeyValue(it->first);

        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " Completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds" << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second" << endl;



        // X. Retrieve 100 random keys
        //BENCHMARK("GET 100 random keys") {
        //  return db->getKeyValue(to_string(rand() % keyValues.size()));
        //};

        // 4. Retrieve the same 100 000 key-value pairs
        // Retrieval speed with a 'warm cache' (if any implemented)

        // 5. Store 50 000 key-value pair UPDATES (so half of the data is 'new') (first half of db)
        // So the performance of half of the key-value pairs may differ

        // 6. Retrieve the same 100 000 key-value pairs
        // Same as 4 if we have a 'write through cache', halfway between 4 and 3 otherwise

        // We know we have been successful when:-
        // We have min,max,var,total time for each group of operations
        // We have saved these results to a csv file for later comparison

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }


    SECTION("perf-100k-memory") {
        cout << "_-_-_-_-_-_-In-memory key-value store performance test-_-_-_-_-_-_" << endl;
        string dbName = "myemptydb";
        unique_ptr<keyValueStore> memoryStore = make_unique<memoryKeyValueStore>(),
            memoryIndexStore = make_unique<memoryKeyValueStore>();
        unique_ptr<Idatabase> db(createFreshDB(dbName, memoryStore, memoryIndexStore));

        int total = 100000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        for (; i < total;i++)
            keyValues.push_back(make_pair(hashedKey(to_string(i)), encodedValue(to_string(i))));

        cout << "Key size is max " << to_string(total - 1).length() << " bytes." << endl;

        long every = 1000;
        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        cout << "_-_-_-_-_-_-SET-_-_-_-_-_-_" << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, move(it->second));
            i++;
            if (!(i % every))
                cout << ".";
        }
        cout << endl;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds." << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;
        cout << endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        string aString("blank");
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++)
            result = db->getKeyValue(it->first);

        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds." << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }

    // Now do the same for pure disc backed storage

    SECTION("Store and Retrieve 100 000 keys - File based key-value store") {
        cout << "_-_-_-_-_-_-File based key-value store performance test-_-_-_-_-_-_" << endl;
        string dbName = "myemptydb",
            fullPath = ".mdb/" + dbName;
        unique_ptr<keyValueStore> memoryStore = make_unique<fileKeyValueStore>(fullPath);
        unique_ptr<Idatabase> db(createFreshDB(dbName, memoryStore));

        int total = 100000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        for (; i < total;i++)
            keyValues.push_back({ hashedKey(to_string(i)), encodedValue(to_string(i)) });

        cout << "Key size is max " << to_string(total - 1).length() << " bytes." << endl;

        long every = 1000;
        // 2. Store 100 000 key-value pairs (no overlap)
        // Raw storage speed
        cout << "_-_-_-_-_-_-SET-_-_-_-_-_-_" << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, move(it->second));
            i++;
            if (!(i % every))
                cout << ".";
        }
        cout << endl;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds." << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;
        cout << endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        string aString("blank");
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++)
            result = db->getKeyValue(it->first);

        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds." << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }
}

TEST_CASE("query-performance", "[!hide][performance][query]") {

    SECTION("Bucket query performance test (In-memory key-value store)") {
        cout << "_-_-_-_-_-In-memory key-value store performance test (Bucket query vs key fetch)-_-_-_-_-_-_" << endl;
        string dbName = "myemptydb";
        unique_ptr<keyValueStore> memoryStore = make_unique<memoryKeyValueStore>(),
            memoryIndexStore = make_unique<memoryKeyValueStore>();
        unique_ptr<Idatabase> db(createFreshDB(dbName, memoryStore, memoryIndexStore));

        int total = 1000000, every = 1000;
        string bucket = "my bucket";
        unordered_set<string> keysInBuckets;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        for (; i < total;i++) {
            keyValues.push_back({ hashedKey(to_string(i)), encodedValue(to_string(i)) });
            if (!(i % every))
                keysInBuckets.insert(to_string(i));
        }
        cout << "Key size is max " << to_string(total - 1).length() << " bytes." << endl;

        // 2. Store key-value pairs (no overlap)
        // Raw storage speed
        cout << "_-_-_-_-_-_-SET-_-_-_-_-_-_" << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            if (0 == i % every)
                db->setKeyValue(it->first, move(it->second), bucket);
            else
                db->setKeyValue(it->first, move(it->second));
            i++;
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in "
            << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0)
            << " seconds." << endl;
        cout << "  "
            << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;
        cout << endl;

        // 3. Retrieve 100 000 key-value pairs (no overlap)
        // Raw retrieval speed
        string aString = "blank";
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET KEYS IN THE BUCKET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keysInBuckets.begin(); it != keysInBuckets.end(); it++)
            result = db->getKeyValue(*it);

        end = chrono::steady_clock::now();
        auto getTimeMicro = (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0);
        cout << "  " << keysInBuckets.size() << " completed in "
            << getTimeMicro
            << " seconds." << endl;
        cout << "  "
            << (keysInBuckets.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " requests per second." << endl;

        // 4. Query comparison
        cout << "_-_-_-_-_-_-QUERY KEYS IN THE BUCKET-_-_-_-_-_-_" << endl;
        BucketQuery bq(bucket);
        cout << "Executing query." << endl;
        begin = chrono::steady_clock::now();
        unique_ptr<IQueryResult> res(db->query(bq));
        cout << "Retrieving results." << endl;
        const KeySet& recordKeys = res->recordKeys();
        end = chrono::steady_clock::now();
        auto queryTimeMicro = (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0);
        cout << "  Query completed in "
            << queryTimeMicro
            << " seconds." << endl;
        cout << "  "
            << (1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count())
            << " queries per second." << endl;
        cout << "Asserting that the query result size should equal the number we placed in buckets." << endl;
        REQUIRE(recordKeys.get()->size() == keysInBuckets.size());
        cout << "Asserting that query time should be quicker than total get time." << endl;
        REQUIRE(queryTimeMicro < getTimeMicro);

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }
}

TEST_CASE("profiling-100k", "[!hide][performance][memory][100k]") {

    SECTION("perf-100k-memory") {
        cout << "_-_-_-_-_-_-In-memory key-value store performance test - 100k-_-_-_-_-_-_" << endl;
        string dbName("myemptydb");
        unique_ptr<keyValueStore> memoryStore = make_unique<memoryKeyValueStore>(),
            memoryIndexStore = make_unique<memoryKeyValueStore>();
        unique_ptr<Idatabase> db(createFreshDB(dbName, memoryStore, memoryIndexStore));

        int total = 100000;
        long every = 1000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        keyValues.reserve(total);
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        for (; i < total;i++) {
            keyValues.push_back(make_pair(hashedKey(to_string(i)), encodedValue(to_string(i))));
            if (0 == i % every)
                cout << ".";
        }
        cout << endl;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;
        cout << endl;
        cout << "Key size is max " << to_string(total - 1).length() << " bytes." << endl;

        // 2. Store key-value pairs (no overlap)
        // Raw storage speed
        cout << "_-_-_-_-_-_-SET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, move(it->second));
            i++;
            if (!(i % every))
                cout << ".";
        }
        cout << endl;
        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;
        cout << endl;

        // 3. Retrieve key-value pairs (no overlap)
        // Raw retrieval speed
        string aString = "blank";
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++)
            result = db->getKeyValue(it->first);

        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }
}

TEST_CASE("profiling-1m", "[!hide][performance][memory][1m]") {

    SECTION("perf-1m-memory") {
        cout << "_-_-_-_-_-_-In-memory key-value store performance test - 1m-_-_-_-_-_-_" << endl;
        string dbName = "myemptydb";
        unique_ptr<keyValueStore> memoryStore = make_unique<memoryKeyValueStore>(),
            memoryIndexStore = make_unique<memoryKeyValueStore>();
        unique_ptr<Idatabase> db(createFreshDB(dbName, memoryStore, memoryIndexStore));

        int total = 1000000;
        long every = 10000;

        // 1. Pre-generate the keys and values in memory (so we don't skew the test)
        vector<pair<hashedKey, encodedValue>> keyValues;
        long i = 0;
        cout << "Pre-generating key value pairs." << endl;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        for (; i < total;i++) {
            keyValues.push_back(make_pair(hashedKey(to_string(i)), encodedValue(to_string(i))));
            if (0 == i % every)
                cout << ".";
        }
        cout << endl;
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;
        cout << endl;
        cout << "Key size is max " << to_string(total - 1).length() << " bytes." << endl;

        // 2. Store key-value pairs (no overlap)
        // Raw storage speed
        cout << "_-_-_-_-_-_-SET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        i = 0;
        for (auto it = keyValues.begin(); it != keyValues.end(); it++) {
            db->setKeyValue(it->first, move(it->second));
            i++;
            if (!(i % every))
                cout << ".";
        }
        cout << endl;
        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;
        cout << endl;

        // 3. Retrieve key-value pairs (no overlap)
        // Raw retrieval speed
        string aString = "blank";
        encodedValue result(aString);
        cout << "_-_-_-_-_-_-GET-_-_-_-_-_-_" << endl;
        begin = chrono::steady_clock::now();
        for (auto it = keyValues.begin(); it != keyValues.end(); it++)
            result = move(db->getKeyValue(it->first));

        end = chrono::steady_clock::now();
        cout << "  " << keyValues.size() << " completed in " << (chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.0) << " seconds." << endl;
        cout << "  " << (keyValues.size() * 1000000.0 / chrono::duration_cast<chrono::microseconds>(end - begin).count()) << " requests per second." << endl;

        // 7. Tear down
        cout << "Tests complete." << endl;
        db->destroy();
    }

} */