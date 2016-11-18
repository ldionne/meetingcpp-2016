// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <metal.hpp>

#include <type_traits>


template <typename T>
using is_void = metal::same<T, void>;

// sample(example)
using Types = metal::list<int, void, char, long, void>;

using NoVoid = metal::remove_if<Types, metal::lambda<is_void>>;
static_assert(std::is_same<NoVoid,
               metal::list<int, char, long>>{});

using Ptrs = metal::transform<metal::lambda<std::add_pointer_t>,Types>;
static_assert(std::is_same<Ptrs,
               metal::list<int*, void*, char*, long*, void*>>{});
// end-sample

int main() { }
