// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/not_equal.hpp>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/category_of.hpp>
#include <boost/fusion/include/define_assoc_struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/vector.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
namespace fusion = boost::fusion;
namespace hana = boost::hana;
namespace mpl = boost::mpl;


template <typename T>
struct is_associative
  : std::is_base_of<
    fusion::associative_tag,
    typename fusion::traits::category_of<T>::type
  >
{ };


template <typename T>
struct is_fusion_associative_sequence
  : mpl::and_<
    fusion::traits::is_sequence<T>,
    is_associative<T>
  >
{ };

template <typename T>
struct is_fusion_nonassociative_sequence
  : mpl::and_<
    fusion::traits::is_sequence<T>,
    mpl::not_<is_associative<T>>
  >
{ };


namespace tags {
  struct brand;
  struct model;
  struct name;
  struct last_name;
  struct age;
}

BOOST_FUSION_DEFINE_ASSOC_STRUCT(, Car,
  (std::string, brand, tags::brand)
  (std::string, model, tags::model)
);

static_assert(is_fusion_associative_sequence<Car>{}, "");
static_assert(!is_fusion_nonassociative_sequence<Car>{}, "");

BOOST_FUSION_DEFINE_ASSOC_STRUCT(, Person,
  (std::string, name, tags::name)
  (std::string, last_name, tags::last_name)
  (int, age, tags::age)
);

static_assert(is_fusion_associative_sequence<Person>{}, "");
static_assert(!is_fusion_nonassociative_sequence<Person>{}, "");

template <typename T>
  std::enable_if_t<std::is_same<T, int>::value,
T> from_json(std::istream& in) {
  T result;
  in >> result;
  return result;
}

template <typename T>
  std::enable_if_t<std::is_same<T, std::string>::value,
T> from_json(std::istream& in) {
  in.ignore(std::numeric_limits<std::streamsize>::max(), '"');
  T result;
  char c;
  while (in.get(c) && c != '"') {
    result += c;
  }
  return result;
}

template <typename Xs>
  std::enable_if_t<is_fusion_associative_sequence<Xs>::value,
Xs> from_json(std::istream& in) {
  Xs result;
  in.ignore(std::numeric_limits<std::streamsize>::max(), '{');
  // TODO: get rid of Hana here
  auto size = hana::size_c<fusion::result_of::size<Xs>{}>;
  size.times.with_index([&](auto i) {
    auto& element = fusion::at_c<i>(result);
    using Element = std::decay_t<decltype(element)>;
    in.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    element = from_json<Element>(in);
    if (i != size - 1u)
      in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
  });
  in.ignore(std::numeric_limits<std::streamsize>::max(), '}');
  return result;
}

template <typename Xs>
  std::enable_if_t<is_fusion_nonassociative_sequence<Xs>::value,
Xs> from_json(std::istream& in) {
  Xs result;
  in.ignore(std::numeric_limits<std::streamsize>::max(), '[');
  // TODO: get rid of Hana here
  auto size = hana::size_c<fusion::result_of::size<Xs>{}>;
  size.times.with_index([&](auto i) {
    auto& element = fusion::at_c<i>(result);
    using Element = std::decay_t<decltype(element)>;
    element = from_json<Element>(in);
    if (i != size - 1u)
      in.ignore(std::numeric_limits<std::streamsize>::max(), ',');
  });
  in.ignore(std::numeric_limits<std::streamsize>::max(), ']');
  return result;
}


bool operator==(Car const& a, Car const& b) {
  return a.brand == b.brand && a.model == b.model;
}

std::string quoted(std::string const& s) {
  return '"' + s + '"';
}

std::ostream& operator<<(std::ostream& os, Car const& c) {
  os << "{" << "\"brand\":" << quoted(c.brand)
     << "," << "\"model\":" << quoted(c.model)
     << "}";
  return os;
}

bool operator==(Person const& a, Person const& b) {
  return a.name == b.name && a.last_name == b.last_name && a.age == b.age;
}

std::ostream& operator<<(std::ostream& os, Person const& p) {
  os << "{" << "\"name\":"      << quoted(p.name)
     << "," << "\"last_name\":" << quoted(p.last_name)
     << "," << "\"age\":"       << p.age
     << "}";
  return os;
}

int main() {
  std::istringstream json(R"EOS(
    [
      {
        "name": "John",
        "last_name": "Doe",
        "age": 30
      },
      {
        "brand": "BMW",
        "model": "Z3"
      },
      {
        "brand": "Audi",
        "model": "A4"
      }
    ]
  )EOS");

  auto actual = from_json<fusion::vector<Person, Car, Car>>(json);

  std::cout << fusion::at_c<0>(actual) << std::endl;
  assert((fusion::at_c<0>(actual) == Person{"John", "Doe", 30}));

  std::cout << fusion::at_c<1>(actual) << std::endl;
  assert((fusion::at_c<1>(actual) == Car{"BMW", "Z3"}));

  std::cout << fusion::at_c<2>(actual) << std::endl;
  assert((fusion::at_c<2>(actual) == Car{"Audi", "A4"}));
}
