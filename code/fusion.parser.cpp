// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/remove_if.hpp>
#include <boost/fusion/include/vector.hpp>

#include <boost/mpl/placeholders.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;


// sample(parser)
template <typename T>
struct parser {
  T operator()(std::istream& in) const {
    T result;
    in >> result;
    return result;
  }
};

template <typename T>
parser<T> parse() { return {}; }
// end-sample

// sample(literal)
struct void_ { };

struct literal_parser {
  char c;
  void_ operator()(std::istream& in) const {
    in.ignore(1, c);
    return {};
  }
};

literal_parser lit(char c) { return {c}; }
// end-sample

// sample(combine)
template <typename ...Parsers>
auto combine_parsers(Parsers const& ...parsers) {
  return [=](std::istream& in) {
    fusion::vector<decltype(parsers(in))...> all{parsers(in)...};
    auto result = fusion::remove_if<std::is_same<mpl::_1, void_>>(all);
    return fusion::as_vector(result);
  };
}
// end-sample

// sample(usage)
int main() {
  auto parser = combine_parsers(
    lit('(') , parse<int>()          ,
    lit(',') , parse<std::string>()  ,
    lit(',') , parse<double>()       ,
    lit(')')
  );

  std::istringstream text{"(1, foo, 3.3)"};
  fusion::vector<int, std::string, double> data = parser(text);

  assert(fusion::at_c<0>(data) == 1);
  assert(fusion::at_c<1>(data) == "foo");
  assert(fusion::at_c<2>(data) == 3.3);
}
// end-sample
