// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>

#include <cassert>
#include <string>
#include <type_traits>
namespace hana = boost::hana;
using namespace std::literals;


template <typename ...T>
using typelist = hana::tuple<hana::type<T>...>;

int main() {
// sample(mpl)
constexpr auto Types = typelist<int, void, char, long>{};

constexpr auto NoVoid = hana::remove_if(Types, hana::traits::is_void);
static_assert(NoVoid == typelist<int, char, long>{}, "");

constexpr auto Pointers = hana::transform(Types,
                                          hana::traits::add_pointer);
static_assert(Pointers == typelist<int*, void*, char*, long*>{}, "");
// end-sample


// sample(fusion)
// tuple
auto tuple = hana::make_tuple(1, 2.2f, "hello"s, 3.4, 'x');
auto no_floats = hana::remove_if(tuple, [](auto const& t) {
  return hana::traits::is_floating_point(hana::typeid_(t));
});

assert(no_floats == hana::make_tuple(1, "hello"s, 'x'));

// map
struct a; struct b; struct c;
auto map = hana::make_map(
  hana::make_pair(hana::type<a>{}, 1),
  hana::make_pair(hana::type<b>{}, 'x'),
  hana::make_pair(hana::type<c>{}, "hello"s)
);

assert(map[hana::type<a>{}] == 1);
assert(map[hana::type<b>{}] == 'x');
assert(map[hana::type<c>{}] == "hello"s);
// end-sample
}
