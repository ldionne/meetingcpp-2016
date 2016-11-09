// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <cassert>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


// sample(struct)
template <typename Event>
struct event_system {
  using Callback = std::function<void()>;
  std::unordered_map<Event, std::vector<Callback>> map_;
// end-sample

// sample(constructor)
explicit event_system(std::initializer_list<Event> events) {
  for (auto const& event : events)
    map_.insert({event, {}});
}
// end-sample

// sample(on)
template <typename F>
void on(Event const& event, F handler) {
  auto callbacks = map_.find(event);
  assert(callbacks != map_.end() &&
    "trying to add a handler to an unknown event");

  callbacks->second.push_back(handler);
}
// end-sample

// sample(trigger)
void trigger(Event const& event) {
  auto callbacks = map_.find(event);
  assert(callbacks != map_.end() &&
    "trying to trigger an unknown event");

  for (auto& handler : callbacks->second)
    handler();
}
// end-sample
};

// sample(usage)
int main() {
  event_system<std::string> events{{"foo", "bar", "baz"}};

  events.on("foo", []() { std::cout << "foo triggered!" << '\n'; });
  events.on("foo", []() { std::cout << "foo again!"     << '\n'; });
  events.on("bar", []() { std::cout << "bar triggered!" << '\n'; });
  events.on("baz", []() { std::cout << "baz triggered!" << '\n'; });
  // events.on("unknown", []() { }); // WOOPS! Runtime error!

  events.trigger("foo");
  events.trigger("baz");
  // events.trigger("unknown"); // WOOPS! Runtime error!
}
// end-sample
