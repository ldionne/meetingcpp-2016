// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <brigand/brigand.hpp>

#include <type_traits>


// sample(example)
using Types = brigand::list<int, void, char, long, void>;

using NoVoid = brigand::remove_if<Types, std::is_void<brigand::_1>>;
static_assert(std::is_same<NoVoid,
               brigand::list<int, char, long>>{}, "");

using Ptrs = brigand::transform<Types, std::add_pointer<brigand::_1>>;
static_assert(std::is_same<Ptrs,
               brigand::list<int*, void*, char*, long*, void*>>{}, "");
// end-sample

int main() { }
