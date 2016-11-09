// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


// sample(before)
auto tuple = hana::make_tuple(1, 2.2f, 3.4, 'x', true);
// end-sample

#if 0
// sample(after)
auto tuple = hana::tuple{1, 2.2f, 3.4, 'x', true};
// end-sample
#endif

int main() { }
