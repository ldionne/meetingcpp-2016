// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/equal.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector_c.hpp>
namespace mpl = boost::mpl;


// sample(dimensions)
// base dimensions                     M  L  T  I  Θ  J  N
using mass        = mpl::vector_c<int, 1, 0, 0, 0, 0, 0, 0>;
using length      = mpl::vector_c<int, 0, 1, 0, 0, 0, 0, 0>;
using time        = mpl::vector_c<int, 0, 0, 1, 0, 0, 0, 0>;
using charge      = mpl::vector_c<int, 0, 0, 0, 1, 0, 0, 0>;
using temperature = mpl::vector_c<int, 0, 0, 0, 0, 1, 0, 0>;
using intensity   = mpl::vector_c<int, 0, 0, 0, 0, 0, 1, 0>;
using amount      = mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 1>;

// composite dimensions
using velocity     = mpl::vector_c<int, 0, 1, -1, 0, 0, 0, 0>; // L/T
using acceleration = mpl::vector_c<int, 0, 1, -2, 0, 0, 0, 0>; // L/T²
using force        = mpl::vector_c<int, 1, 1, -2, 0, 0, 0, 0>; // ML/T²
// end-sample

// sample(dimensions-compare)
template <typename A, typename B>
constexpr bool compatible = mpl::equal<A, B>::value;
// end-sample

// sample(quantity)
template <typename Dimensions>
struct quantity {
  double value_;

  explicit quantity(double v) : value_(v) { }

  template <typename OtherDimensions>
  explicit quantity(quantity<OtherDimensions> other)
    : value_(other.value_)
  {
    static_assert(compatible<Dimensions, OtherDimensions>,
      "Constructing quantities with incompatible dimensions!");
  }

  explicit operator double() const { return value_; }
};
// end-sample

// sample(dimensions-compose)
template <typename D1, typename D2>
auto operator*(quantity<D1> a, quantity<D2> b) {
  using D = typename mpl::transform<D1, D2, mpl::plus<>>::type;
  return quantity<D>{static_cast<double>(a) * static_cast<double>(b)};
}

template <typename D1, typename D2>
auto operator/(quantity<D1> a, quantity<D2> b) {
  using D = typename mpl::transform<D1, D2, mpl::minus<>>::type;
  return quantity<D>{static_cast<double>(a) / static_cast<double>(b)};
}

template <typename D1, typename D2>
auto operator+(quantity<D1> a, quantity<D2> b) {
  static_assert(compatible<D1, D2>,
    "Adding quantities with incompatible dimensions!");
  return quantity<D1>{static_cast<double>(a) + static_cast<double>(b)};
}

// etc..
// end-sample

#if 0
// sample(usage)
quantity<mass>         m{10.3};
quantity<length>       d{3.6};
quantity<time>         t{2.4};
quantity<velocity>     v{d / t};
quantity<acceleration> a{...};
quantity<force>        f{m * v}; // Compiler error!
quantity<force>        f{m * a}; // Works as expected
// end-sample
#endif

int main() {
  quantity<mass>         m{10.3};
  quantity<length>       d{3.6};
  quantity<time>         t{2.4};
  quantity<velocity>     v{d / t};
  quantity<acceleration> a{3.9};
  quantity<force>        f{m * a};

  quantity<force> f2 = f + f;
}
