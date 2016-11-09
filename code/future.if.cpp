// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/not.hpp>
#include <boost/hana/type.hpp>

#include <iostream>
#include <type_traits>
namespace hana = boost::hana;


namespace sample_before {
// sample(before)
auto has_xyz = hana::is_valid([](auto&& t) -> decltype(t.xyz) { });

template <typename T>
auto f(T const& t) -> std::enable_if_t<decltype(has_xyz(t)){}> {
  std::cout << "t has a member named xyz: " << t.xyz << std::endl;
}

template <typename T>
auto f(T const& t) -> std::enable_if_t<decltype(!has_xyz(t)){}> {
  std::cout << "t does not have a member named xyz" << std::endl;
}
// end-sample
}

#if 0
// sample(after)
template <typename T>
void f(T const& t) {
  auto has_xyz = hana::is_valid([](auto&& x) -> decltype(x.xyz) { });
  if constexpr(has_xyz(t)) {
    std::cout << "t has a member named xyz: " << t.xyz << std::endl;
  } else {
    std::cout << "t does not have a member named xyz" << std::endl;
  }
}
// end-sample
#endif

struct Foo { int xyz; };

int main() {
  Foo foo{3};
  int no_xyz = 4;

  sample_before::f(foo);
  sample_before::f(no_xyz);
}
