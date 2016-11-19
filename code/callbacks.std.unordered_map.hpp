// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#ifndef CODE_CALLBACKS_STD_UNORDERED_MAP_HPP
#define CODE_CALLBACKS_STD_UNORDERED_MAP_HPP

#include <cassert>
#include <functional>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>


// sample(struct)
struct event_system {
  using Callback = std::function<void()>;
  std::unordered_map<std::string, std::vector<Callback>> map_;
// end-sample

// sample(constructor)
explicit event_system(std::initializer_list<std::string> events) {
  for (auto const& event : events)
    map_.insert({event, {}});
}
// end-sample

// sample(on)
template <typename F>
void on(std::string const& event, F callback) {
  auto callbacks = map_.find(event);
  assert(callbacks != map_.end() &&
    "trying to add a callback to an unknown event");

  callbacks->second.push_back(callback);
}
// end-sample

// sample(trigger)
void trigger(std::string const& event) const {
  auto callbacks = map_.find(event);
  assert(callbacks != map_.end() &&
    "trying to trigger an unknown event");

  for (auto& callback : callbacks->second)
    callback();
}
// end-sample
};

#endif
