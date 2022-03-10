#include "extensions/highwayhash.h"
#include "hashes.h"
#include "types.h"

namespace mdbHashes {

  class defaultHash::impl {
  public:
    impl() {}
    impl(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4) : memberHasher(s1, s2, s3, s4) {}

    mdbHH::highwayHash memberHasher;
  };

  defaultHash::defaultHash()
    : memberImpl(std::make_unique<impl>()) {}

  defaultHash::defaultHash(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4) :memberImpl(std::make_unique<impl>(s1, s2, s3, s4)) {}

  defaultHash::~defaultHash() {}

  std::size_t defaultHash::operator() (const std::string& s) const noexcept {
    return memberImpl->memberHasher(s);
  }

  std::size_t defaultHash::operator() (const char* data, std::size_t length) const noexcept {
    return memberImpl->memberHasher(data, length);
  }

  std::size_t defaultHash::operator() (const mdbTypes::hashedValue& s) const noexcept {
    return memberImpl->memberHasher(s);
  }

  std::size_t defaultHash::operator() (const encodedValue& s) const noexcept {
    return memberImpl->memberHasher(s);
  }

  std::size_t defaultHash::operator() (const std::vector<std::byte>& bytes) const noexcept {
    return memberImpl->memberHasher(bytes);
  }

}