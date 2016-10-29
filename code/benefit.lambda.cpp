// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/filter.hpp>
#include <boost/hana/or.hpp>
#include <boost/hana/traits.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


// sample(mpl)
using Types = mpl::vector<int, float const, char volatile, long const>;
using CV_Types = mpl::copy_if<Types,
                    mpl::or_<std::is_volatile<mpl::_1>,
                             std::is_const<mpl::_1>>>::type;
// end-sample

static_assert(mpl::equal<
  CV_Types,
  mpl::vector<float const, char volatile, long const>
>::value, "");


template <typename ...T>
using typelist = hana::tuple<hana::type<T>...>;

// sample(hana)
auto Types = typelist<int, float const, char volatile, long const>{};
auto CV_Types = hana::filter(Types, [](auto t) {
  return hana::traits::is_volatile(t) || hana::traits::is_const(t);
});
// end-sample

BOOST_HANA_CONSTANT_CHECK(CV_Types == typelist<float const, char volatile, long const>{});

int main() { }
