// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/set.hpp>


int main() {
  {
    auto hello = make_node([] { });
    auto world = make_node([] { });
    auto g = make_graph(
      hello >> world
    );
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(hello) == hana::make_set());
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(world) == hana::make_set(hello));
  }

  {
    auto a = make_node([] { });
    auto b = make_node([] { });
    auto c = make_node([] { });
    auto d = make_node([] { });
    auto g = make_graph(
      a >> b,
      b >> c,
      c >> d
    );
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(a) == hana::make_set());
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(b) == hana::make_set(a));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(c) == hana::make_set(a, b));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(d) == hana::make_set(a, b, c));
  }

  {
    auto a = make_node([] { });
    auto b1 = make_node([] { });
    auto b2 = make_node([] { });
    auto c = make_node([] { });
    auto g = make_graph(
      a >> b1,
      a >> b2,
      b1 >> c,
      b2 >> c
    );
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(a) == hana::make_set());
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(b1) == hana::make_set(a));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(b2) == hana::make_set(a));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(c) == hana::make_set(a, b1, b2));
  }

  {
    auto a = make_node([] { });
    auto b1 = make_node([] { });
    auto b2 = make_node([] { });
    auto c = make_node([] { });
    auto d = make_node([] { });
    auto g = make_graph(
      a >> b1,
      a >> b2,
      b1 >> c,
      b2 >> c,
      c >> d
    );
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(a) == hana::make_set());
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(b1) == hana::make_set(a));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(b2) == hana::make_set(a));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(c) == hana::make_set(a, b1, b2));
    BOOST_HANA_CONSTANT_CHECK(g.dependencies(d) == hana::make_set(a, b1, b2, c));
  }
}
