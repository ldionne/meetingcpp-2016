// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "callbacks.std.unordered_map.hpp"

#include <iostream>
#include <string>


// sample(usage)
int main() {
  event_system events{{"foo", "bar", "baz"}};

  events.on("foo", []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo", []() { std::cout << "foo again!"     << '\n'; });
  events.on("bar", []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz", []() { std::cout << "baz triggered!" << '\n'; });

  events.trigger("foo");
  events.trigger("baz");
  // events.trigger("unknown"); // WOOPS! Runtime error!
}
// end-sample
