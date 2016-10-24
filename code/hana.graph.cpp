// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"

#include <boost/hana/equal.hpp>
#include <boost/hana/tuple.hpp>
#include <cassert>
namespace hana = boost::hana;


// sample(usage)
auto square = make_node([](int v) { return v * v; });
auto cube   = make_node([](int v) { return v * v * v; });
auto sum    = make_node([](int x, int y) { return x + y; });
auto twice  = make_node([](int x) { return x * 2; });

auto g = make_graph(
  input >> (square, cube),
  (square, cube) >> sum,
  cube >> twice,
  (twice, sum) >> output
);

int main() {
  for (int i = 0; i < 10; ++i) {
    auto result = g(i);
    assert(result == hana::make_tuple(i*i*i * 2, i*i + i*i*i));
  }
}
// end-sample
