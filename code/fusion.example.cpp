// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/comparison.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/remove_if.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/placeholders.hpp>

#include <cassert>
#include <string>
#include <type_traits>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;
using namespace std::literals;


int main() {
// sample(example)
// vector
auto vector = fusion::make_vector(1, 2.2f, "hello"s, 3.4, 'x');
auto no_floats = fusion::remove_if<
                        std::is_floating_point<mpl::_>>(vector);

assert(no_floats == fusion::make_vector(1, "hello"s, 'x'));

// map
struct a; struct b; struct c;
auto map = fusion::make_map<a, b, c>(1, 'x', "hello"s);

assert(fusion::at_key<a>(map) == 1);
assert(fusion::at_key<b>(map) == 'x');
assert(fusion::at_key<c>(map) == "hello"s);
// end-sample
}
