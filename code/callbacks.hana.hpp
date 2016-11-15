// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#ifndef CODE_CALLBACKS_HANA_HPP
#define CODE_CALLBACKS_HANA_HPP

#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL
#include <boost/hana.hpp>

#include <cassert>
#include <functional>
#include <string>
#include <unordered_map>
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

// sample(construct-runtime)
std::unordered_map<std::string, std::vector<Callback>*> dynamic_;

event_system() {
  hana::for_each(hana::keys(map_), [&](auto event) {
    dynamic_.insert({hana::to<char const*>(event), &map_[event]});
  });
}
//end-sample

// sample(trigger-runtime)
void trigger(std::string const& e) const {
  auto callbacks = dynamic_.find(e);
  assert(callbacks != dynamic_.end() &&
    "trying to trigger an unknown event");

  for (auto& handler : *callbacks->second)
    handler();
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
