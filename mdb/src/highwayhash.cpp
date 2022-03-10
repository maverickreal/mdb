#include "extensions/highwayhash.h"

namespace mdbHH {

  using namespace highwayhash;

  highwayHash::highwayHash()
    : memberKey{ 1,2,3,4 },
    memberHH(new HighwayHashCatT<HH_TARGET>(memberKey)),
    memberResult(new HHResult64()){}

  highwayHash::highwayHash(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4)
    : memberKey{ s1,s2,s3,s4 },
    memberHH(new HighwayHashCatT<HH_TARGET>(memberKey)),
    memberResult(new HHResult64()){}

  highwayHash::~highwayHash() {
    //delete m_hh;
    //delete m_result; // for windows - why does this help???
  }

  std::size_t highwayHash::operator() (std::string const& s) const noexcept {
    memberHH->Reset(memberKey);
    memberHH->Append(s.c_str(), s.length());
    memberHH->Finalize(memberResult);
    return *memberResult;
  }

  std::size_t highwayHash::operator() (const char* data, std::size_t length) const noexcept {
    memberHH->Reset(memberKey);
    memberHH->Append(data, length);
    memberHH->Finalize(memberResult);
    return *memberResult;
  }

  std::size_t highwayHash::operator() (const hashedValue& data) const noexcept {
    return data.hash();
    // TODO ensure the hash has the same basis as that required by this class
  }

  std::size_t highwayHash::operator() (const encodedValue& data) const noexcept {
    return data.hash();
    // TODO ensure the hash has the same basis as that required by this class
  }

  std::size_t highwayHash::operator() (const bytes& data) const noexcept {
    memberHH->Reset(memberKey);
    for (auto it = data.begin(); it != data.end(); ++it)
      memberHH->Append((const char*)&*it, sizeof(*it));
    memberHH->Finalize(memberResult);
    return *memberResult;
  }
}