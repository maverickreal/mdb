#ifndef CRYPT_H
#define CRYPT_H

#include "aesImpl.h"
#include "algorithm"

using namespace std;

static const vector<unsigned char> key = { 0x0c, 0x0d, 0x0e, 0x0f ,0x00, 0x01, 0x02, 0x06, 0x07, 0x08, 0x03, 0x04, 0x05, 0x09, 0x0a, 0x0b };
static AES aes(AESKeyLength::AES_128);

namespace crypt {
    class Crypt {
    public:
        static string encrypt(const string& s) {
            int sz = s.length(), i = 0;
            sz += 16 - (sz % 16);

            vector<unsigned char> res, plain(sz, ' ');
            for (auto it = s.crbegin(); it != s.crend(); ++it)
                plain[i++] = *it;

            res = aes.EncryptECB(plain, key);
            string str;
            for (const unsigned char& c : res)
                str.append(to_string(short(c)) + ",");
            return str;
        }

        static string decrypt(const string& s) {
            vector<unsigned char>v;
            string tmp;

            for (const char& ch : s) {
                if (ch == ',') {
                    v.push_back(stoi(tmp));
                    tmp.clear();
                }
                else
                    tmp.push_back(ch);
            }

            v = aes.DecryptECB(v, key);

            while (!v.empty() && v.back() == ' ')
                v.pop_back();
            return string(v.begin(), v.end());
        }
    };
}
#endif