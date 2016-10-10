// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#ifndef HANA_GRAPH_HPP
#define HANA_GRAPH_HPP

#include <boost/hana/any_of.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/flatten.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/prepend.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/sort.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>

#include <future>
#include <type_traits>
#include <utility>
namespace hana = boost::hana;


template <typename F, typename ...Dependencies>
struct computation : F {
  explicit computation(F const& f, Dependencies const& ...deps)
    : F{f}, dependencies{deps...}
  { }

  hana::tuple<Dependencies...> dependencies;
};

template <typename F, typename ...Dependencies>
auto make_computation(F const& f, Dependencies const& ...dependencies) {
  return computation<F, Dependencies...>{f, dependencies...};
}

template <typename F1, typename ...D1, typename F2, typename ...D2>
auto operator==(computation<F1, D1...> const& a, computation<F2, D2...> const& b) {
  return std::is_same<F1, F2>{};
}

template <typename F1, typename ...D1, typename F2, typename ...D2>
auto operator!=(computation<F1, D1...> const& a, computation<F2, D2...> const& b) {
  return hana::not_(std::is_same<F1, F2>{});
}

template <typename F, typename G>
auto depends_on(F const& f, G const& g) {
  return hana::eval_if(hana::contains(f.dependencies, g),
    [] { return hana::true_c; },
    [&](auto _) {
      return hana::any_of(_(f).dependencies, [&](auto const& dep) {
        return depends_on(dep, g);
      });
    }
  );
}

template <typename Fs>
auto linearized(Fs const& fs) {
  return hana::sort(fs, [](auto const& f, auto const& g) {
    return depends_on(g, f);
  });
}

template <typename F>
auto all_dependencies(F const& f) {
  auto other_deps = hana::flatten(hana::transform(f.dependencies, [](auto const& dep) {
    return all_dependencies(dep);
  }));
  return hana::to_tuple(hana::to_set(hana::concat(f.dependencies, other_deps)));
}

template <typename F, typename G>
auto independent(F const& f, G const& g) {
  return !depends_on(f, g) && !depends_on(g, f);
}

template <typename F, typename G>
auto circular(F const& f, G const& g) {
  return depends_on(f, g) && depends_on(g, f);
}

template <typename F>
auto run_parallel(F const& f, std::launch policy) {
  auto fs = linearized(hana::prepend(all_dependencies(f), f));
  auto independent_groups = hana::group(fs, [](auto const& a, auto const& b) {
    return independent(a, b);
  });

  hana::for_each(independent_groups, [policy](auto const& group) {
    auto futures = hana::transform(group, [&](auto const& f) {
      return std::async(policy, f);
    });
    hana::for_each(std::move(futures), [](auto const& f) {
      f.wait();
    });
  });
};

template <typename Lock, typename F>
auto locked(Lock& lock, F const& f) {
  return [&lock, f]() {
    std::lock_guard<Lock> guard{lock};
    return f();
  };
}

#endif // !HANA_GRAPH_HPP
