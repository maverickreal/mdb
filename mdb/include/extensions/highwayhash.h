#ifndef HIGHWAYHASH_H
#define HIGHWAYHASH_H

#include "string"
#include "../types.h"
#include "../highwayhash/highwayhash.h"

using namespace mdbTypes;
namespace mdbHH {
  class highwayHash {
  public:
    highwayHash();
    highwayHash(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4);
    ~highwayHash();

    std::size_t operator() (const hashedValue& s) const noexcept;
    std::size_t operator() (const mdbTypes::encodedValue& s) const noexcept;
    std::size_t operator() (const std::string& s) const noexcept;
    std::size_t operator() (const bytes& bytes) const noexcept;
    std::size_t operator() (const char* data, std::size_t length) const noexcept;

  private:
    highwayhash::HHKey memberKey HH_ALIGNAS(64);
    highwayhash::HighwayHashCatT<HH_TARGET>* memberHH;
    highwayhash::HHResult64* memberResult;
  };
}
#endif
