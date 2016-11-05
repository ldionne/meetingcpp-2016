// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/not.hpp>
#include <boost/hana/traits.hpp>
#include <boost/hana/type.hpp>

#include <string>
#include <type_traits>
namespace hana = boost::hana;


namespace std {
  template <typename T>
  using void_t = void;
}

namespace cpp14_sample {
// sample(cpp14)
template <typename T, typename = void>
struct has_xxx : std::false_type { };

template <typename T>
struct has_xxx<T, std::void_t<decltype(&T::xxx)>> : std::true_type { };

struct Foo { int xxx; };
static_assert(has_xxx<Foo>::value, "");
static_assert(!has_xxx<std::string>::value, "");
// end-sample
}

namespace hana_sample {
// sample(hana)
auto has_xxx = hana::is_valid([](auto t)
    -> decltype(hana::traits::declval(t).xxx)
{ });

struct Foo { int xxx; };
static_assert(has_xxx(hana::type<Foo>{}), "");
static_assert(!has_xxx(hana::type<std::string>{}), "");
// end-sample
}

int main() { }
