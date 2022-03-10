#ifndef IS_CONTAINER_H
#define IS_CONTAINER_H

#include "valarray"
#include "type_traits"
#include "tuple"
// Healthy Reminder -> https://www.geeksforgeeks.org/templates-cpp/
// http://www.cplusplus.com/reference/type_traits/decay/
namespace mdbContainer {

  template <typename T>
  struct isPair {
    typedef typename std::decay_t<T> testType;

    template <typename A>
    static constexpr bool test(A* pt, decltype(pt->first)* = nullptr,
      decltype(pt->second)* = nullptr) {
      typedef typename A::first_type first_type;
      typedef typename A::second_type second_type;

      return std::is_same_v<A, std::pair<first_type, second_type>>;
    }

    template <typename A>
    static constexpr bool test(...) {
      return false;
    }
    static const bool value = test<testType>(nullptr);
  };

  template <typename T>
  struct isContainer {
    typedef typename std::decay_t<T> testType;

    template <typename A>
    static constexpr bool test(A* pt, A const* cpt = nullptr,
      decltype(pt->begin())* = nullptr, decltype(pt->end())* = nullptr,
      decltype(cpt->begin())* = nullptr, decltype(cpt->end())* = nullptr) {
      typedef typename A::iterator iterator;
      typedef typename A::const_iterator const_iterator;
      typedef typename A::value_type value_type;
      return std::is_same<decltype(pt->begin()), iterator>::value &&
        std::is_same<decltype(pt->end()), iterator>::value &&
        std::is_same<decltype(cpt->begin()), const_iterator>::value &&
        std::is_same<decltype(cpt->end()), const_iterator>::value &&
        !isPair<decltype(*pt->begin())>::value;
    }

    template <typename A>
    static constexpr bool test(...) {
      return false;
    }

    static const bool value = test<testType>(nullptr);
  };

  template <typename T, std::size_t N>
  struct isContainer<T[N]> : std::true_type {};

  template <std::size_t N>
  struct isContainer<char[N]> : std::false_type {};

  template <typename T>
  struct isContainer<std::valarray<T>> : std::true_type {};

  template <typename... Args>
  struct isContainer<std::tuple<Args...>> : std::true_type {};

  template <typename T>
  struct is_keyed_container {

    typedef typename std::decay_t<T> testType;
    template <typename A>
    static constexpr bool test(A* pt, A const* cpt = nullptr,
      decltype(pt->begin())* = nullptr, decltype(pt->end())* = nullptr,
      decltype(cpt->begin())* = nullptr, decltype(cpt->end())* = nullptr) {
      typedef typename A::iterator iterator;
      typedef typename A::const_iterator const_iterator;
      typedef typename A::value_type value_type;
      return std::is_same<decltype(pt->begin()), iterator>::value &&
        std::is_same<decltype(pt->end()), iterator>::value &&
        std::is_same<decltype(cpt->begin()), const_iterator>::value &&
        std::is_same<decltype(cpt->end()), const_iterator>::value &&
        isPair<decltype(*pt->begin())>::value;
    }

    template <typename A>
    static constexpr bool test(...) {
      return false;
    }

    static const bool value = test<testType>(nullptr);
  };

}

#endif
