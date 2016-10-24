// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"


// An error will be triggered if the type `should_be` is constructed with
// does not match `T`.
template <typename T>
struct should_be { explicit should_be(T) { } };

auto a = make_node([]{}); using A = decltype(a);
auto b = make_node([]{}); using B = decltype(b);
auto c = make_node([]{}); using C = decltype(c);
auto d = make_node([]{}); using D = decltype(d);
auto e = make_node([]{}); using E = decltype(e);


int main() {
  should_be<edge<A, B>>{a >> b};

  should_be<edge<A, hana::tuple<B, C>>>{a >> (b, c)};
  should_be<edge<A, hana::tuple<B, C, D>>>{a >> (b, c, d)};

  should_be<edge<hana::tuple<A, B>, E>>{(a, b) >> e};
  should_be<edge<hana::tuple<A, B, C>, E>>{(a, b, c) >> e};
  should_be<edge<hana::tuple<A, B, C, D>, E>>{(a, b, c, d) >> e};
}
