// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

// ~/code/llvm/build/prefix/bin/clang++ -I ~/code/hana/include -isystem ~/code/llvm/build/prefix/lib/clang/4.0.0/include -I ~/code/llvm/tools/clang/reflection -Xclang -freflection -std=c++1z code/reflection.cpp

#include <boost/hana.hpp>

#include <iostream>
#include <reflexpr>
#include <type_traits>
namespace hana = boost::hana;
using namespace hana::literals;
namespace meta = std::meta;


template <typename T, typename ...>
struct get_size : meta::get_size<T> { };

template <typename T, std::size_t I, typename ...>
struct get_element : meta::get_element<T, I> { };

template <typename T, typename ...>
struct get_data_members : meta::get_data_members<T> { };

template <typename T, typename ...>
struct get_base_name : meta::get_base_name<T> { };

template <typename T, typename ...>
struct get_pointer : meta::get_pointer<T> { };


template <typename MetaObject>
struct reflexpr_wrapper;

template <typename MetaObjectSeq>
struct reflexpr_sequence_wrapper;

template <typename T>
constexpr auto reflexpr_ = reflexpr_wrapper<reflexpr(T)>{};

template <typename MetaObjectSeq>
struct reflexpr_sequence_wrapper {
  template <typename ...Empty>
  constexpr auto size() const {
    return get_size<MetaObjectSeq, Empty...>{};
  }

  template <typename I>
  constexpr auto operator[](I const&) const {
    using Element = typename get_element<MetaObjectSeq, I::value>::type;
    return reflexpr_wrapper<Element>{};
  }
};

template <typename MetaObject>
struct reflexpr_wrapper {
  template <typename ...Empty>
  constexpr auto members() const {
    using Members = typename get_data_members<MetaObject, Empty...>::type;
    return reflexpr_sequence_wrapper<Members>{};
  }

  template <typename ...Empty>
  constexpr auto name() const {
    return get_base_name<MetaObject, Empty...>::value;
  }

  template <typename ...Empty>
  constexpr auto pointer() const {
    return get_pointer<MetaObject, Empty...>::value;
  }
};



template <typename T>
std::ostream& to_json(std::ostream& out, T const& v) {
  if constexpr(std::meta::Record<reflexpr(T)>) {
    out << "{";
    auto members = reflexpr_<T>.members();
    auto indices = hana::make_range(0_c, members.size());
    hana::for_each(indices, [&](auto i) {
      if (i > 0) out << ", ";
      out << '"' << members[i].name() << "\": ";
      to_json(out, v.*members[i].pointer());
    });
    out << '}';
  } else {
    out << v;
  }
  return out;
}


struct point { float x, y, z; };
struct triangle { point a, b, c; };

struct tetrahedron {
  triangle base;
  point apex;
};

int main() {
  tetrahedron t{
    {{0.f,0.f,0.f}, {1.f,0.f,0.f}, {0.f,0.f,1.f}},
    {0.f,1.f,0.f}
  };

  to_json(std::cout, t);
}
