// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.callbacks.hpp"

#include <iostream>
#include <string>
namespace hana = boost::hana;
using namespace hana::literals;


std::string read_from_stdin() {
  return "foo";
}

// sample(usage)
int main() {
  auto events = make_event_system("foo"_s, "bar"_s, "baz"_s);

  events.on("foo"_s, []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo"_s, []() { std::cout << "foo again!" << '\n'; });
  events.on("bar"_s, []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz"_s, []() { std::cout << "baz triggered!" << '\n'; });

  std::string e = read_from_stdin();
  events.trigger(e);
}
// end-sample
