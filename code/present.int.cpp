// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <brigand/brigand.hpp>

#include <array>
#include <cstddef>


// sample(integral_constant)
template <typename T, T v>
struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
// end-sample

namespace sample_classic {
// sample(operator-classic)
template <typename X, typename Y>
struct plus;

template <typename T, T x, T y>
struct plus<integral_constant<T, x>, integral_constant<T, y>>
  : integral_constant<T, x + y>
{ };
// end-sample

// sample(multiply_dimensions-classic)
template <typename D1, typename D2>
struct multiply_dimensions;

template <typename ...D1, typename ...D2>
struct multiply_dimensions<brigand::list<D1...>, brigand::list<D2...>>{
  using type = brigand::list<typename plus<D1, D2>::type...>;
};
// end-sample

static_assert(std::is_same<
  multiply_dimensions<
    brigand::list<integral_constant<int, 1>, integral_constant<int, 2>>,
    brigand::list<integral_constant<int, 3>, integral_constant<int, 0>>
  >::type,
  brigand::list<integral_constant<int, 4>, integral_constant<int, 2>>
>{});
}

// sample(operator-hana)
template <typename T, T x, T y>
constexpr auto
operator+(integral_constant<T, x>, integral_constant<T, y>)
{ return integral_constant<T, x + y>{}; }
// end-sample

namespace sample_hana {
// sample(multiply_dimensions-hana)
template <typename D1, typename D2>
struct multiply_dimensions;

template <typename ...D1, typename ...D2>
struct multiply_dimensions<brigand::list<D1...>, brigand::list<D2...>>{
  using type = brigand::list<decltype(D1{} + D2{})...>;
};
// end-sample

static_assert(std::is_same<
  multiply_dimensions<
    brigand::list<integral_constant<int, 1>, integral_constant<int, 2>>,
    brigand::list<integral_constant<int, 3>, integral_constant<int, 0>>
  >::type,
  brigand::list<integral_constant<int, 4>, integral_constant<int, 2>>
>{}, "");
}

constexpr int to_int(char c) {
  return static_cast<int>(c) - static_cast<int>('0');
}

template <char ...chars>
constexpr int parse() {
  constexpr char arr[] = {chars...};
  constexpr std::size_t N = sizeof...(chars);

  int base = 10, multiplier = 1, number = 0;
  for (std::size_t i = 0; i < N; ++i) {
    char c = arr[N - i - 1];
    number += to_int(c) * multiplier;
    multiplier *= base;
  }
  return number;
}

// sample(literal-operator)
template <char ...c>
constexpr auto operator"" _c() {
  constexpr int n = parse<c...>();
  return integral_constant<int, n>{};
}

auto n = 10_c + 30_c; // n is integral_constant<int, 40>
// end-sample

// sample(use-literal-operator)
template <typename Size>
auto array_iota(Size size) {
  std::array<int, size> array; // Yes, this is valid!
  for (int i = 0; i < size; ++i)
    array[i] = i;
  return array;
}

auto array = array_iota(10_c + 30_c);
// end-sample

int main() { }
