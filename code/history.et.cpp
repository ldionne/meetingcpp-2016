// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#include <cassert>
#include <initializer_list>
#include <utility>
#include <vector>


template <typename T>
struct Matrix {
  explicit Matrix(std::initializer_list<std::initializer_list<T>> init) {
    for (auto const& row : init)
      storage_.emplace_back(row);
  }

  auto operator[](std::pair<std::size_t, std::size_t> index) const {
    return storage_[index.first][index.second];
  }

  std::vector<std::vector<T>> storage_;
};

#if 0
// sample(main)
template <typename T>
struct Matrix { /* ... */ };

// end-sample
#endif


// sample(def)
template <typename E1, typename E2>
struct Sum {
  E1 const& left_;
  E2 const& right_;

  auto operator[](std::pair<std::size_t, std::size_t> index) const
  { return left_[index] + right_[index]; }
};

template <typename Left, typename Right>
Sum<Left, Right> operator+(Left const& left, Right const& right)
{ return {left, right}; }
// end-sample

// sample(usage)
int main() {
  Matrix<int> m1{{ 1,  2,  3}, { 4,  5,  6}, { 7,  8,  9}};
  Matrix<int> m2{{11, 12, 13}, {14, 15, 16}, {17, 18, 19}};
  auto sum = m1 + m2;  // Nothing done yet!
  int x = sum[{1, 2}]; // The work is done here
  assert(x == 6 + 16);
}
// end-sample
