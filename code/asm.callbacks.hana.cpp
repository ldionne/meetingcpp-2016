// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "../code/hana.callbacks.hpp"

namespace hana = boost::hana;
using namespace hana::literals;


template <typename Events>
__attribute__((noinline)) void trigger(Events const& events) {
  events.trigger("event1"_s);
}

int main() {
  auto events = make_event_system("event1"_s,
                                  "event2"_s,
                                  "event3"_s,
                                  "event4"_s,
                                  "event5"_s);

  events.on("event1"_s, []{});
  events.on("event2"_s, []{});
  events.on("event3"_s, []{});
  events.on("event4"_s, []{});
  events.on("event5"_s, []{});

  trigger(events);
}
