// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/remove_if.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/traits.hpp>
namespace hana = boost::hana;


// sample(before)
constexpr auto tuple = hana::make_tuple(1, 2.2f, 3.4, 'x', true);

// Can't be contexpr because of the lambda; very inconsistent
auto no_floats = hana::remove_if(tuple, [](auto const& t) {
  return hana::traits::is_floating_point(hana::typeid_(t));
});
// end-sample

#if 0
// sample(after)
constexpr auto tuple = hana::make_tuple(1, 2.2f, 3.4, 'x', true);

// Can be constexpr now; much better
constexpr auto no_floats = hana::remove_if(tuple, [](auto const& t) {
  return hana::traits::is_floating_point(hana::typeid_(t));
});
// end-sample
#endif

int main() { }
