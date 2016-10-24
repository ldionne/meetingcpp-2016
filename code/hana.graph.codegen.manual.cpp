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

  auto ff1 = std::async(LAUNCH_POLICY, f1);
  auto ff2 = std::async(LAUNCH_POLICY, f2);
  auto ff3 = std::async(LAUNCH_POLICY, f3);
  ff1.wait(); ff2.wait(); ff3.wait();

  auto fg = std::async(LAUNCH_POLICY, g);
  fg.wait();

  auto fh = std::async(LAUNCH_POLICY, h);
  auto fi = std::async(LAUNCH_POLICY, i);
  fh.wait(); fi.wait();

  auto fj = std::async(LAUNCH_POLICY, j);
  fj.wait();
}
