#include "types.h"
#include "extensions/highwayhash.h"
#include "algorithm"
#include "type_traits"
#include "typeinfo"

namespace mdbTypes {

  hashedValue::hashedValue(const bytes& data, std::size_t length, std::size_t hash) {
    memberHasValue = true;
    memberData = data;
    memberLength = length;
    memberHash = hash;
  }

  hashedValue::hashedValue() {
    memberHasValue = false,
      memberLength = 0,
      memberHash = 0;
  }

  /** Copy/move constuctors and operators **/
  hashedValue::hashedValue(const hashedValue& from) {
    memberHasValue = from.memberHasValue;
    memberData = from.memberData;
    memberLength = from.memberLength;
    memberHash = from.memberHash;
  }

  hashedValue::hashedValue(const hashedValue&& from) {
    memberHasValue = from.memberHasValue;
    memberData = from.memberData;
    memberLength = from.memberLength;
    memberHash = from.memberHash;
    //std::cout << "hashedValue::move-ctor" << std::endl;
  }

  hashedValue& hashedValue::operator=(const hashedValue& other) {
    memberHasValue = other.memberHasValue;
    memberData = other.memberData;
    memberLength = other.memberLength;
    memberHash = other.memberHash;
    return *this;
  }

  /** Conversion constuctors **/
  hashedValue::hashedValue(const encodedValue& from) {
    memberHasValue = from.hasValue();
    memberData = from.data();
    memberLength = from.length();
    memberHash = from.hash();
  }

  hashedValue::hashedValue(encodedValue&& from) {
    memberHasValue = from.hasValue();
    memberData = std::move(from.data());
    memberLength = from.length();
    memberHash = from.hash();
  }

  const bytes hashedValue::data() const {
    return memberData;
  }

  std::size_t hashedValue::length() const {
    return memberLength;
  }

  std::size_t hashedValue::hash() const {
    return memberHash;
  }

  bool hashedValue::hasValue() const {
    return memberHasValue;
  }

  bool hashedValue::operator==(const hashedValue& other) const {
    if (memberHash != other.memberHash)
      return false;
    // compare hash first as generally it will be faster most often
    if (memberLength != other.memberLength)
      return false;
    // only do a data wise comparison if you must (highly, highly unlikely - requires hash collision)
    //return 0 == std::strcmp(m_data,other.m_data);
    return memberData == other.memberData;
  }

  bool hashedValue::operator!=(const hashedValue& other) const {
    return !(*this == other);
  }

  bool encodedValue::operator==(const encodedValue& other) const {
    return memberHasValue == other.memberHasValue && memberType == other.memberType && memberValue == other.memberValue;
  }

  bool encodedValue::operator!=(const encodedValue& other) const {
    return !(*this == other);
  }

  std::ostream& operator<<(std::ostream& os, const Type t) {
    os << (int)t;
    return os;
  }

  std::istream& operator>>(std::istream& is, Type t) {
    int tint;
    is >> tint;
    t = (Type)tint;
    return is;
  }

}
