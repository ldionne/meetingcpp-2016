// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"


auto a = make_node([]{}); using A = decltype(a);
auto b = make_node([]{}); using B = decltype(b);
auto c = make_node([]{}); using C = decltype(c);
auto d = make_node([]{}); using D = decltype(d);
auto e = make_node([]{}); using E = decltype(e);


int main() {
  // has_target
  {
    edge<A, B> e1;
    static_assert(has_target(e1, b), "");
    static_assert(!has_target(e1, c), "");

    edge<A, hana::tuple<B, C>> e2;
    static_assert(has_target(e2, b), "");
    static_assert(has_target(e2, c), "");
    static_assert(!has_target(e2, d), "");

    edge<A, hana::tuple<B, C, D>> e3;
    static_assert(has_target(e3, b), "");
    static_assert(has_target(e3, c), "");
    static_assert(has_target(e3, d), "");
    static_assert(!has_target(e3, e), "");
  }

  // has_source
  {
    edge<A, E> e1;
    static_assert(has_source(e1, a), "");
    static_assert(!has_source(e1, b), "");

    edge<hana::tuple<A, B>, E> e2;
    static_assert(has_source(e2, a), "");
    static_assert(has_source(e2, b), "");
    static_assert(!has_source(e2, c), "");

    edge<hana::tuple<A, B, C>, E> e3;
    static_assert(has_source(e3, a), "");
    static_assert(has_source(e3, b), "");
    static_assert(has_source(e3, c), "");
    static_assert(!has_source(e3, d), "");
  }
}
