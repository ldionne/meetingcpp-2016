// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <type_traits>
namespace hana = boost::hana;


namespace classic {
// sample(classic)
using IntPtr = std::add_pointer<int>::type;

static_assert(std::is_same<IntPtr, int*>::value, "");
// end-sample
}


namespace bootstrap {
// sample(operators)
template <typename T>
struct type { };

template <typename T>
constexpr type<T*> add_pointer(type<T> const&)
{ return {}; }

template <typename T, typename U>
constexpr std::integral_constant<bool, false>
operator==(type<T> const&, type<U> const&)
{ return {}; }

template <typename T>
constexpr std::integral_constant<bool, true>
operator==(type<T> const&, type<T> const&)
{ return {}; }
// end-sample

// sample(use-operators)
constexpr auto IntPtr = add_pointer(type<int>{});

static_assert(IntPtr == type<int*>{}, "");
// end-sample
}


namespace use_hana {
using hana::tuple;
using hana::type;

// sample(typelist)
constexpr auto Types = tuple<
  type<int>, type<char>, type<float>, type<char>, type<void>
>{};

constexpr auto NoChar = hana::remove(Types, type<char>{});
constexpr auto Uniqued = hana::unique(Types);
constexpr auto Reversed = hana::reverse(Types);
// etc...
// end-sample
}

namespace use_hana_sugar {
using hana::tuple;
using hana::type;

// sample(typelist-sugar)
template <typename ...T>
using typelist = tuple<type<T>...>;

constexpr auto Types = typelist<int, char, float, char, void>{};

constexpr auto NoChar = hana::remove(Types, type<char>{});
constexpr auto Uniqued = hana::unique(Types);
constexpr auto Reversed = hana::reverse(Types);
// etc...
// end-sample
}

int main() { }