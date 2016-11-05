// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#ifndef CODE_HANA_CALLBACKS_HPP
#define CODE_HANA_CALLBACKS_HPP

#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
#include <boost/hana.hpp>

#include <cassert>
#include <functional>
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

// sample(trigger-runtime)
  void trigger(std::string const& e) {
    bool found = false;
    hana::for_each(hana::keys(this->map_), [&](auto const& event) {
      if (!found && hana::to<char const*>(event) == e) {
        this->trigger(event);
        found = true;
      }
    });

    assert(found && "triggering unknown event");
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

#endif
