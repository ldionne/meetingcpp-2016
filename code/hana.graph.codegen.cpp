// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"

#include <future>
#include <iostream>
#include <mutex>


static constexpr std::launch LAUNCH_POLICY = std::launch::deferred;

template <typename Lock, typename F>
auto locked(Lock& lock, F const& f) {
  return [&lock, f]() {
    std::lock_guard<Lock> guard{lock};
    return f();
  };
}

int main() {
  std::mutex io;
  auto f1 = make_node(locked(io, [] { std::cout << "f1" << std::endl; }));
  auto f2 = make_node(locked(io, [] { std::cout << "f2" << std::endl; }));
  auto f3 = make_node(locked(io, [] { std::cout << "f3" << std::endl; }));
  auto g  = make_node(locked(io, [] { std::cout << "g" << std::endl; }));
  auto h  = make_node(locked(io, [] { std::cout << "h" << std::endl; }));
  auto i  = make_node(locked(io, [] { std::cout << "i" << std::endl; }));
  auto j  = make_node(locked(io, [] { std::cout << "j" << std::endl; }));

  auto process = make_graph(
    (f1, f2, f3) >> g,
    g >> (h, i),
    (h, i) >> j
  );

  process(LAUNCH_POLICY);
}
