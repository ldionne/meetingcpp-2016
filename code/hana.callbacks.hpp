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


template <typename ...Pairs>
using map = decltype(hana::make_map(std::declval<Pairs>()...));

// sample(struct)
template <typename ...Events>
struct event_system {
  using Callback = std::function<void()>;
  map<hana::pair<Events, std::vector<Callback>>...> map_;
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
void trigger(std::string const& e) const {
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
void trigger(Event e) const {
  auto is_known_event = hana::contains(map_, e);
  static_assert(is_known_event,
    "trying to trigger an unknown event");

  for (auto& handler : this->map_[e])
    handler();
}
// end-sample
};

// sample(constructor)
template <typename ...Events>
event_system<Events...> make_event_system(Events ...events) {
  return {};
}
// end-sample

#endif
