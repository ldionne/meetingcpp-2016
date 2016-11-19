// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <type_traits>
namespace hana = boost::hana;


namespace classic {
// sample(classic)
template <typename T>
struct add_pointer { using type = T*; };

template <typename T, typename U>
struct is_same : std::false_type { };

template <typename T>
struct is_same<T, T> : std::true_type { };
// end-sample

// sample(before)
using IntPtr = add_pointer<int>::type;
static_assert(is_same<IntPtr, int*>{});
// end-sample
}


namespace bootstrap {
// sample(operators)
template <typename T>
struct type { };

template <typename T>
constexpr type<T*> add_pointer(type<T>)
{ return {}; }

template <typename T, typename U>
constexpr std::false_type operator==(type<T>, type<U>)
{ return {}; }

template <typename T>
constexpr std::true_type operator==(type<T>, type<T>)
{ return {}; }
// end-sample

// sample(after)
constexpr auto IntPtr = add_pointer(type<int>{});
static_assert(IntPtr == type<int*>{});
// end-sample
}


namespace use_hana {
using hana::tuple;
using hana::type;

// sample(typelist)
constexpr auto Types = tuple<
  type<int>, type<char>, type<float>, type<char>, type<void>
>{};
// end-sample
}

namespace use_hana_sugar {
using hana::tuple;
using hana::type;

// sample(typelist)

// or
template <typename ...T>
constexpr auto tuple_t = tuple<type<T>...>{};

constexpr auto Types = tuple_t<int, char, float, char, void>;
// end-sample
}

int main() { }
