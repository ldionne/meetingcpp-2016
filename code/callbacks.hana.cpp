// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "callbacks.hana.hpp"

#include <iostream>
namespace hana = boost::hana;
using namespace hana::literals;


// sample(usage)
int main() {
  auto events = make_event_system("foo"_s, "bar"_s, "baz"_s);

  events.on("foo"_s, []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo"_s, []() { std::cout << "foo again!" << '\n'; });
  events.on("bar"_s, []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz"_s, []() { std::cout << "baz triggered!" << '\n'; });
  // events.on("unknown"_s, []() { }); // compiler error!

  events.trigger("foo"_s); // no overhead for event lookup
  events.trigger("baz"_s);
  // events.trigger("unknown"_s); // compiler error!
}
// end-sample
