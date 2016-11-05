// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
#include <boost/hana.hpp>

#include <functional>
#include <iostream>
#include <vector>
namespace hana = boost::hana;
using namespace hana::literals;


// sample(struct)
template <typename EventMap>
struct event_system {
  EventMap map_;
// end-sample

// sample(on)
  template <typename Event, typename F>
  void on(Event e, F handler) {
    auto is_known_event = hana::contains(map_, e);
    static_assert(is_known_event,
      "trying to add a handler to an unknown event");

    map_[e].push_back(handler);
  }
// end-sample

// sample(trigger)
  template <typename Event>
  void trigger(Event e) {
    auto is_known_event = hana::contains(map_, e);
    static_assert(is_known_event,
      "trying to trigger an unknown event");

    for (auto& handler : this->map_[e])
      handler();
  }
};
// end-sample

// sample(make_event_system)
template <typename ...Events>
auto make_event_system(Events ...events) {
  using Callbacks = std::vector<std::function<void()>>;
  using Map = decltype(hana::make_map(
    hana::make_pair(events, Callbacks{})...
  ));

  return event_system<Map>{};
}
// end-sample


// sample(usage)
int main() {
  auto events = make_event_system("foo"_s, "bar"_s, "baz"_s);

  events.on("foo"_s, []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo"_s, []() { std::cout << "foo again!" << '\n'; });
  events.on("bar"_s, []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz"_s, []() { std::cout << "baz triggered!" << '\n'; });
  // events.on("unknown"_s, []() { }); // compiler error!

  events.trigger("foo"_s);
  events.trigger("baz"_s);
  // events.trigger("unknown"_s); // compiler error!
}
// end-sample
