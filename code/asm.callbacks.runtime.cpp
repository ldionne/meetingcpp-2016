// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "../code/callbacks.hpp"
#include <string>


template <typename Events>
__attribute__((noinline)) void trigger(Events const& events) {
  events.trigger("event1");
}

int main() {
  event_system<std::string> events{{"event1", "event2", "event3", "event4", "event5"}};

  events.on("event1", []{});
  events.on("event2", []{});
  events.on("event3", []{});
  events.on("event4", []{});
  events.on("event5", []{});

  trigger(events);
}
