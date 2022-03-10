#ifndef HASHES_H
#define HASHES_H

#include "string"
#include "memory"

namespace mdbHashes {

  class HashedValue;
  class encodedValue;
  class defaultHash {
  public:
    defaultHash();
    // Healthy Reminder -> https://en.cppreference.com/w/cpp/types/integer
    defaultHash(std::uint64_t s1, std::uint64_t s2, std::uint64_t s3, std::uint64_t s4);
    ~defaultHash();

    std::size_t operator() (const mdbTypes::hashedValue& s) const noexcept;
    std::size_t operator() (const encodedValue& s) const noexcept;
    std::size_t operator() (const std::string& s) const noexcept;
    std::size_t operator() (const std::vector<std::byte>& bytes) const noexcept;
    std::size_t operator() (const char* data, std::size_t length) const noexcept;

    class impl;

  private:
    std::unique_ptr<impl> memberImpl;
  };

}
#endif
