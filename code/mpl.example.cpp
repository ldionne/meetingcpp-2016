// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/equal.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>

#include <type_traits>
namespace mpl = boost::mpl;


// sample(example)
using Types = mpl::vector<int, void, char, long, void>;

using NoVoid = mpl::remove_if<Types, std::is_void<mpl::_1>>::type;
static_assert(mpl::equal<mpl::vector<int, char, long>,
                         NoVoid>::value, "");

using Pointers = mpl::transform<Types, std::add_pointer<mpl::_>>::type;
static_assert(mpl::equal<mpl::vector<int*, void*, char*, long*, void*>,
                         Pointers>::value, "");
// end-sample

int main() { }
