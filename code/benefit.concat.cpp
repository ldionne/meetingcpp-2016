// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
#include <boost/hana/equal.hpp>
#include <boost/hana/string.hpp>

#include <cassert>
#include <string>
namespace hana = boost::hana;
using namespace hana::literals;
using namespace std::literals;


// sample(concatenate)
template <typename S1, typename S2>
constexpr auto concat(S1 const& s1, S2 const& s2) {
  return s1 + s2;
}

template <typename S1, typename S2, typename ...Sn>
constexpr auto concat(S1 const& s1, S2 const& s2, Sn const& ...sn) {
  return s1 + concat(s2, sn...);
}

int main() {
  std::string strings1 = concat("Hello"s, " "s, "World"s);
  assert(strings1 == "Hello World"s);

  constexpr auto strings2 = concat("Hello"_s, " "_s, "World"_s);
  static_assert(strings2 == "Hello World"_s);
}
// end-sample
