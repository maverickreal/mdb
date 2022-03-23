#ifndef CRYPT_H
#define CRYPT_H

#include "aesImpl.h"
#include "algorithm"

using namespace std;

namespace crypt {
    class Crypt {
    public:
        static string encrypt(const string& s) {
            AES aes(AESKeyLength::AES_128);
            vector<unsigned char> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

            int sz = s.length();
            sz += 16 - (sz % 16);

            vector<unsigned char> plain(sz, ' ');
            for (auto it = s.crbegin(); it != s.crend(); ++it)
                plain[--sz] = *it;

            vector<unsigned char>res = aes.EncryptECB(plain, key);
            return string(res.begin(), res.end());
        }

        static string decrypt(const string& s) {
            AES aes(AESKeyLength::AES_128);
            vector<unsigned char> key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f },
                res = aes.DecryptECB(vector<unsigned char>(s.begin(), s.end()), key);

            while(!res.empty() && res.back() == ' ')
                res.pop_back();
            return string(res.begin(), res.end());
        }
    };
}
#endif