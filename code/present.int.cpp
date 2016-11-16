// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <cstddef>
#include <iostream>
#include <type_traits>


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

// sample(operators)
template <typename T, T x, T y>
constexpr auto
operator+(integral_constant<T, x>, integral_constant<T, y>)
{ return integral_constant<T, x + y>{}; }

template <typename T, T x, T y>
constexpr auto
operator==(integral_constant<T, x>, integral_constant<T, y>)
{ return integral_constant<bool, x == y>{}; }

// ...
// end-sample

// sample(use-operators-1)
static_assert(decltype(
  integral_constant<int, 1>{} + integral_constant<int, 4>{}
                              ==
                 integral_constant<int, 5>{}
)::value, "");
// end-sample

// sample(use-operators-2)
static_assert(
  integral_constant<int, 1>{} + integral_constant<int, 4>{}
                              ==
                 integral_constant<int, 5>{}
, "");
// end-sample

// sample(use-operators-3)
template <char ...c>
constexpr auto operator"" _c() {
  constexpr int number = parse<c...>();
  return integral_constant<int, number>{};
}

// end-sample sample(use-operators-3) sample(after)
static_assert(1_c + 4_c == 5_c, "");
// end-sample

// sample(classic)
template <typename X, typename Y>
struct plus;

template <typename T, T x, T y>
struct plus<integral_constant<T, x>, integral_constant<T, y>> {
  using type = integral_constant<T, x + y>;
};

// end-sample sample(before) sample(classic)
static_assert(std::is_same<
  plus<integral_constant<int, 1>, integral_constant<int, 4>>::type,
  integral_constant<int, 5>
>::value, "");
// end-sample

// sample(constexpr)
static_assert(1 + 4 == 5, "");
// end-sample

// sample(overload)
template <typename X, typename Y>
auto f(X x, Y y) -> std::enable_if_t<(x == y)> {
  std::cout << "x == y is known at compile-time" << std::endl;
}

template <typename X, typename Y>
auto f(X x, Y y) -> std::enable_if_t<(x != y)> {
  std::cout << "x != y is known at compile-time" << std::endl;
}

int main() {
  f(3_c, 3_c); // first overload taken
  f(4_c, 3_c); // second overload taken

#if 0 // end-sample sample(overload)
  int i;
  std::cin >> i;
  f(i, 3_c); // ERROR! The value of 'i' is only known at runtime!
  f(3, 3_c); // ERROR! No distinction between 'runtime' and
             //       'compile-time' int.
#endif // end-sample sample(overload)
}
// end-sample
