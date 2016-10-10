// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include "hana.graph.hpp"

#include <future>
#include <iostream>
#include <mutex>


static constexpr std::launch LAUNCH_POLICY = std::launch::deferred;

int main() {
  std::mutex io;
  auto f1 = make_computation(locked(io, [] { std::cout << "f1" << std::endl; }));
  auto f2 = make_computation(locked(io, [] { std::cout << "f2" << std::endl; }));
  auto f3 = make_computation(locked(io, [] { std::cout << "f3" << std::endl; }));
  auto g  = make_computation(locked(io, [] { std::cout << "g" << std::endl; }), f1, f2, f3);
  auto h  = make_computation(locked(io, [] { std::cout << "h" << std::endl; }), g);
  auto i  = make_computation(locked(io, [] { std::cout << "i" << std::endl; }), g);
  auto j  = make_computation(locked(io, [] { std::cout << "j" << std::endl; }), i, h);

  run_parallel(j, LAUNCH_POLICY);
}
