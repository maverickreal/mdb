#ifndef TYPES_H
#define TYPES_H

#include "cstdint"
#include "cstddef"
#include "vector"
#include "string"
#include "iostream"
#include "unordered_set"
#include "functional"
#include "numeric"
#include "algorithm"
#include "container.h"
#include "hashes.h"

namespace mdbTypes {
  using bytes = std::vector<std::byte>;
  template<class T>
  class key {
  private:
    bytes memberData;
  public:
    key(const T& t) {
      memberData.reserve(t.size());
      // Healthy Reminder -> https://www.cplusplus.com/reference/iterator/back_inserter/
      // https://www.cplusplus.com/reference/algorithm/transform/
      // https://www.geeksforgeeks.org/static_cast-in-c-type-casting-operators/
      std::transform(t.begin(), t.end(), std::back_inserter(memberData),
        [](auto c) { return static_cast<std::byte>(c); });
    }
    virtual ~key() = default;

    const bytes& data() const {
      return memberData;
    }

    std::size_t length() const {
      return memberData.size();
    }

    bool operator==(const key<T>& other) const {
      return memberData == other.memberData;
    }

    bool operator!=(const key<T>& other) const {
      return !*this == other;
    }
  };

  class encodedValue;

  // Healthy Reminder -> https://www.cplusplus.com/reference/type_traits/is_constructible/
  template <class U, class T>
  struct isExplicitlyConvertible {
    enum { value = std::is_constructible<T, U>::value && !std::is_convertible<U, T>::value };
  };

  class hashedValue {
  private:
    bool memberHasValue;
    bytes memberData; // Binary representation of the original key data
    std::size_t memberLength; // binary length in bytes
    std::size_t memberHash; // one-way hash of the memebrData using the server' specified algorithm
  public:
    //hashedValue(bytes data,int length,std::size_t hash);
    hashedValue(const bytes& data, std::size_t length, std::size_t hash);
    hashedValue();
    template<class T>
    hashedValue(const key<T>& from) : memberHasValue(true) {
      memberLength = from.length();
      //memberData.reserve(memberLength);
      //from.copy_to(memberData.begin());

      memberData = from.data(); // copy ctor

      // for (auto& d : from.value()) {
      //   memberData.push_back((std::byte)d);
      // }
      // TODO use correct hasher for current database connection, with correct initialisation settings
      defaultHash h1{};
      memberHash = h1(from.data());
    }

    /** Copy/move constuctors and operators **/
    //hashedValue(hashedValue& from);
    hashedValue(const hashedValue& from);
    hashedValue(const hashedValue&& from);
    hashedValue& operator=(const hashedValue& other);

    /** Conversion constuctors **/
    hashedValue(const encodedValue& from);
    hashedValue(encodedValue&& from);

    /** Standard type convenience constructors **/
    /**
   * Accept any basic type. Templated to minimise coding.
   * Good basic reference on techniques used here:
   * https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp
   */
    template <typename VT> //, typename = std::enable_if_t<isExplicitlyConvertible<VT,hashedValue>::value>>
    hashedValue(VT from) : memberHasValue(true), memberData(), memberLength(0), memberHash(0) {
      // first remove reference
      auto v = std::decay_t<VT>(from);
      // second check if its a basic type and convert to bytes
      // Healthy Reminder -> https://www.educative.io/edpresso/what-is-the-constexpr-keyword-in-cpp
      if constexpr (std::is_same_v<std::string, decltype(v)>) {
        //std::cout << "DECLTYPE std::string" << std::endl;
        memberLength = from.length();
        memberData.reserve(memberLength);
        std::transform(from.begin(), from.end(), std::back_inserter(memberData),
          [](auto c) { return static_cast<std::byte>(c); });

      }
      else if constexpr (std::is_same_v<char*, decltype(v)>) {
        //std::cout << "DECLTYPE char ptr" << std::endl;
        std::string s(v);
        memberLength = s.length();
        memberData.reserve(memberLength);
        std::transform(s.begin(), s.end(), std::back_inserter(memberData),
          [](auto c) { return static_cast<std::byte>(c); });

      }
      else if constexpr (std::is_same_v<const char*, decltype(v)>) {
        //std::cout << "DECLTYPE char array" << std::endl;
        std::string s(v);
        memberLength = s.length();
        memberData.reserve(memberLength);
        std::transform(s.begin(), s.end(), std::back_inserter(memberData),
          [](auto c) { return static_cast<std::byte>(c); });

      }
      else if constexpr (std::numeric_limits<VT>::is_integer) {
        //std::cout << "DECLTYPE numeric" << std::endl;
        memberLength = sizeof(v) / sizeof(std::byte);
        auto vcopy = v; // copy ready for modification
        memberData.reserve(memberLength);
        for (auto i = std::size_t(0); i < memberLength; i++)
        {
          // NOTE This implements little-endian conversion TODO do we want this?
          memberData.push_back(static_cast<std::byte>((vcopy & 0xFF)));
          vcopy = vcopy >> sizeof(std::byte);
        }

      }
      else if constexpr (std::is_floating_point_v<VT>) {
        //std::cout << "DECLTYPE float" << std::endl;
        memberLength = sizeof(v) / sizeof(std::byte);
        auto vcopy = v; // copy ready for modification
        unsigned int asInt = *((int*)&vcopy);
        memberData.reserve(memberLength);
        for (auto i = std::size_t(0); i < memberLength; i++)
        {
          // NOTE This implements little-endian conversion TODO do we want this?
          memberData.push_back(static_cast<std::byte>((asInt & 0xFF)));
          asInt = asInt >> sizeof(std::byte);
        }

      }
      else if constexpr (std::is_same_v<bytes, decltype(v)>) {
        memberLength = from.size();
        memberData = std::move(from);

      }
      else if constexpr (is_container<decltype(v)>::value) {
        // Convert contents to encodedValue and serialise it
        // TODO in future this will preserve its runtime type
        bytes data;
        // ^^^ can't reserve yet - may be dynamic type in container 
        //     (We MUST do a deep copy)
        for (auto it = v.begin();it != v.end();++it) {
          hashedValue hv(*it);
          // ^^^ uses template functions, so a little recursion here
          bytes t = hv.data();
          data.insert(std::end(data), t.begin(), t.end());
          // https://stackoverflow.com/questions/2551775/appending-a-vector-to-a-vector
        }
        // TODO append encodedValue(CPP,decltype<VT>,...,data) instead
        memberData.insert(std::end(memberData), data.begin(), data.end());
        memberLength = memberData.size(); // can only know after inserts
      // } else if constexpr (is_keyed_container<decltype(v)>::value) {
      //   std::cout << "DECLTYPE keyed container" << std::endl;

      }
      else if constexpr (is_keyed_container<decltype(v)>::value) {
        // Convert contents to encodedValue and serialise it
        // TODO in future this will preserve its runtime type
        bytes data;
        // ^^^ can't reserve yet - may be dynamic type in container 
        //     (We MUST do a deep copy)
        for (auto it = v.begin();it != v.end();++it) {
          hashedValue hv(it->second);
          // ^^^ uses template functions, so a little recursion here
          bytes t = hv.data();
          data.insert(std::end(data), t.begin(), t.end());
          // https://stackoverflow.com/questions/2551775/appending-a-vector-to-a-vector
        }
        // TODO append encodedValue(CPP,decltype<VT>,...,data) instead
        memberData.insert(std::end(memberData), data.begin(), data.end());
        memberLength = memberData.size(); // can only know after inserts

  /*
      } else if constexpr(std::is_convertible_v<decltype(v),hashedValue> && !std::is_same_v<decltype(v),hashedValue>) {
        hashedValue hv = v; // calls type cast operator - https://www.cplusplus.com/doc/tutorial/typecasting/
        memberHasValue = hv.memberHasValue;
        memberLength = hv.memberLength;
        memberData = std::move(hv.memberData);
        memberHash = hv.memberHash;
        return; // no need to recompute hash

        */
      }
      else {
        throw std::runtime_error(typeid(v).name());
        // TODO we don't support it, fire off a compiler warning
        //static_assert(false, "Must be a supported type, or convertible to std::vector<std::byte>!");
      }
      // TODO use correct hasher for current database connection, with correct initialisation settings
      defaultHash h1{ 1, 2, 3, 4 };
      memberHash = h1(memberData);
    }

    virtual ~hashedValue() = default;
    const bytes data() const;
    std::size_t length() const;
    std::size_t hash() const;
    bool hasValue() const;

    // define << and >> operators
    //friend std::ofstream& operator<<(std::ofstream& out, const hashedValue& from);

    // TODO >>
    // TODO assign(iterator,iteratorend) function to load data from disk

    // equals
    bool operator==(const hashedValue& other) const;
    bool operator!=(const hashedValue& other) const;

  };

  /*
  std::ofstream& operator<<(std::ofstream& out, const hashedValue& from) {
    out << from.memberHash << from.memberHasValue << from.memberLength << from.memberData; // TODO verify this is correct for a char* (memberData)
    return out;
  }
  */

  using hashedKey = hashedValue; // Alias for semantic ease

  using keySet = std::unique_ptr<std::unordered_set<hashedValue>>;

  enum class Type {
    UNKNOWN = 0,
    KEY = 1,
    SET = 2,
    CPP = 3 // TODO determine if we can do this and use refelection, or if we need another way of refering to C++ types
  };

  std::ostream& operator<<(std::ostream& os, const Type t);
  std::istream& operator>>(std::istream& is, Type t);

  /* @brief The encodedValue class is a Value type, intended to be copied cheaply. */

  class encodedValue {
  private:
    bool memberHasValue;
    Type memberType; // internal groundupdb type identifier
    hashedValue memberValue; // same internal representation as a hashedKey, so re-using definition
  public:
    //encodedValue(Type type,bytes data,int length,std::size_t hash): memberHasValue(true), memberType(type), memberValue(data,length,hash) {}
    encodedValue(Type type, const bytes& data, std::size_t length, std::size_t hash) : memberHasValue(true), memberType(type), memberValue(data, length, hash) {}
    encodedValue() : memberHasValue(false), memberType(Type::UNKNOWN), memberValue() {}

    /** Copy/move constuctors and operators **/
    //encodedValue(encodedValue& from) : memberHasValue(from.memberHasValue), memberType(from.memberType), memberValue(from.memberValue) {}
    encodedValue(const encodedValue& from) : memberHasValue(from.memberHasValue), memberType(from.memberType), memberValue(from.memberValue) {}
    encodedValue(const encodedValue&& from) : memberHasValue(from.memberHasValue), memberType(from.memberType), memberValue(std::move(from.memberValue)) {
      //std::cout << "encodedValue::move-ctor" << std::endl;
    }
    encodedValue& operator=(const encodedValue& other) {
      memberHasValue = other.memberHasValue;
      memberType = other.memberType;
      memberValue = other.memberValue;
      return *this;
    };

    // convenience conversion
    encodedValue(const std::string& from) : memberHasValue(true), memberType(Type::CPP), memberValue(hashedValue{ from }) {}

    /** Conversion constuctors **/
    template<typename VT, typename = std::enable_if_t<!isExplicitlyConvertible<VT, hashedValue>::value && !std::is_same_v<VT, hashedValue>>>
    encodedValue(const VT& from) : memberHasValue(true), memberType(Type::CPP), memberValue(hashedValue{ from }) {}

    /** Class methods **/
    Type type() const { return memberType; }
    const bytes data() const { return memberValue.data(); }
    std::size_t length() const { return memberValue.length(); }
    std::size_t hash() const { return memberValue.hash(); }
    bool hasValue() const { return memberHasValue; }

    // define << operator
    //friend std::ofstream& operator<<(std::ofstream& out, const encodedValue& from);
    // TODO >> operator

    // equals
    bool operator==(const encodedValue& other) const;
    bool operator!=(const encodedValue& other) const;
  };

  /*
  std::ofstream& operator<<(std::ofstream& out, const encodedValue& from) {
    out << from.memberType.c_str() << std::endl;
    out << from.memberHasValue;
    if (from.memberHasValue) {
      out << from.memberValue;
    }
    return out;
  }
  */

  using Set = std::unique_ptr<std::unordered_set<encodedValue>>;
}

// std::hash support for classes in this file
namespace std
{
  template<>
  struct hash<encodedValue>
  {
    size_t operator()(const encodedValue& v) const
    {
      std::size_t hv = 0;
      for (auto& vpart : v.data())
        hv = hv ^ (std::to_integer<std::size_t>(vpart) << 1);
      return hv;
    }
  };
  template<>
  struct hash<mdbTypes::hashedValue>
  {
    size_t operator()(const mdbTypes::hashedValue& v) const
    {
      std::size_t hv = 0;
      for (auto& vpart : v.data())
        hv = hv ^ (std::to_integer<std::size_t>(vpart) << 1);
      return hv;
    }
  };
  template<typename T>
  struct hash<mdbTypes::key<T>>
  {
    size_t operator()(const key<T>& v) const
    {
      std::size_t hv = 0;
      for (auto& vpart : v.data())
        hv = hv ^ (std::to_integer<std::size_t>(vpart) << 1);
      return hv;
    }
  };
}
#endif