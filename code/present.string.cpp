// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#define BOOST_MPL_LIMIT_STRING_SIZE 44
#include <boost/hana.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/string.hpp>
#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


namespace mpl_example {
// sample(classic)
// Note: mpl::string is basically represented as
template <char ...c> struct string { };

using Hello = mpl::string<'h', 'e', 'l', 'l', 'o'>;
using World = mpl::string<' ', 'w', 'o', 'r', 'l', 'd'>;

using Hello_world = mpl::insert_range<
                          Hello, mpl::end<Hello>::type, World>::type;
// end-sample

static_assert(mpl::equal<
  Hello_world,
  mpl::string<'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>
>::value, "");
} // end mpl_example


template <char ...c> struct string { };

// sample(operators)
template <char ...c1, char ...c2>
constexpr auto operator==(string<c1...>, string<c2...>) {
  return std::is_same<string<c1...>, string<c2...>>{};
}

template <char ...c1, char ...c2>
constexpr auto operator+(string<c1...>, string<c2...>) {
  return string<c1..., c2...>{};
}
// end-sample

namespace sample1 {
// sample(use-operators)
constexpr auto Hello = string<'h', 'e', 'l', 'l', 'o'>{};
constexpr auto World = string<' ', 'w', 'o', 'r', 'l', 'd'>{};

constexpr auto Hello_world = Hello + World;
// end-sample

static_assert(Hello_world == string<'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>{});
}

// sample(literal)
template <typename CharT, CharT ...c>
constexpr string<c...> operator"" _s() { return {}; }

constexpr auto Hello_world = "hello"_s + " world"_s;
static_assert(Hello_world == "hello world"_s);
// end-sample

int main() { }
