// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>

#include <functional>
#include <iostream>
#include <vector>
namespace fusion = boost::fusion;


// sample(event_system-1)
template <typename EventMap>
struct event_system {
  EventMap map_;
// end-sample

// sample(event_system-3)
  template <typename Event>
  static constexpr bool is_known_event =
    fusion::result_of::has_key<EventMap, Event>::value;
// end-sample

// sample(event_system-2)
  template <typename Event, typename F>
  void on(F handler) {
    static_assert(is_known_event<Event>,
      "trying to add a handler to an unknown event");

    fusion::at_key<Event>(map_).push_back(handler);
  }
// end-sample

// sample(event_system-4)
  template <typename Event>
  void trigger() {
    static_assert(is_known_event<Event>,
      "trying to trigger an unknown event");

    for (auto& handler : fusion::at_key<Event>(map_))
      handler();
  }
};
// end-sample

// sample(make_event_system)
template <typename ...Events>
auto make_event_system() {
  using Callbacks = std::vector<std::function<void()>>;
  using Map = fusion::map<fusion::pair<Events, Callbacks>...>;
  return event_system<Map>{};
}
// end-sample

// sample(usage)
struct foo;
struct bar;
struct baz;

int main() {
  auto events = make_event_system<foo, bar, baz>();

  events.on<foo>([]() { std::cout << "foo triggered!" << '\n'; });
  events.on<foo>([]() { std::cout << "foo again!"     << '\n'; });
  events.on<bar>([]() { std::cout << "bar triggered!" << '\n'; });
  events.on<baz>([]() { std::cout << "baz triggered!" << '\n'; });
  // events.on<struct unknown>([]() { }); // compiler error!

  events.trigger<foo>();
  events.trigger<baz>();
  // events.trigger<struct unknown>(); // compiler error!
}
// end-sample
