#include "string"
#include "vector"
#include "extensions/types.h"

using namespace std;
using namespace ext;

namespace inc {
    pair<uint8_t*, int> hashKey(const long long& n);

    pair<uint8_t*, int> hashKey(const string& s);

    pair<uint8_t*, int> hashKey(const Date& d);
};
