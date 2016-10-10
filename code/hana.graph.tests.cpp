// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
  {
    auto hello = make_computation([] { });
    auto world = make_computation([] { }, hello);
    BOOST_HANA_CONSTANT_ASSERT(depends_on(world, hello));
  }

  {
    auto hello = make_computation([] { });
    auto world = make_computation([] { }, hello);
    BOOST_HANA_CONSTANT_ASSERT(
      linearized(hana::make_tuple(world, hello)) ==
      hana::make_tuple(hello, world));
  }

  {
    auto hello = make_computation([] { });
    auto world = make_computation([] { }, hello);
    BOOST_HANA_CONSTANT_ASSERT(world.dependencies == hana::make_tuple(hello));
  }

  {
    auto f = make_computation([] { });
    auto g = make_computation([] { }, f);
    auto h = make_computation([] { }, g);
    auto i = make_computation([] { }, g);

    BOOST_HANA_CONSTANT_ASSERT(depends_on(g, f));
    BOOST_HANA_CONSTANT_ASSERT(!depends_on(f, g));

    BOOST_HANA_CONSTANT_ASSERT(depends_on(h, g));
    BOOST_HANA_CONSTANT_ASSERT(depends_on(h, f));
    BOOST_HANA_CONSTANT_ASSERT(!depends_on(g, h));
    BOOST_HANA_CONSTANT_ASSERT(!depends_on(f, h));

    BOOST_HANA_CONSTANT_ASSERT(
      linearized(hana::make_tuple(h, g, f, i)) == hana::make_tuple(f, g, h, i)
    );
  }
}
