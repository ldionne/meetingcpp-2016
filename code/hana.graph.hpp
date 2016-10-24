// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.

#ifndef HANA_GRAPH_HPP
#define HANA_GRAPH_HPP

#include <boost/hana.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <future>
#include <type_traits>
#include <utility>
namespace hana = boost::hana;


//////////////////////////////////////////////////////////////////////////////
// Node
//////////////////////////////////////////////////////////////////////////////
template <typename F>
struct node {
  constexpr node() = default;
  constexpr explicit node(F const& f) { }

  // TODO: implement based on F
  void operator()() const {

  }
};

template <typename F, typename G>
constexpr auto operator==(node<F> const&, node<G> const&) {
  return std::is_same<F, G>{};
}

template <typename F, typename G>
constexpr auto operator!=(node<F> const&, node<G> const&) {
  return hana::not_(std::is_same<F, G>{});
}

template <typename F>
constexpr auto make_node(F const& f) {
  return node<F>{f};
}

//////////////////////////////////////////////////////////////////////////////
// Edge
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename G>
struct edge { };

template <typename F, typename G, typename H>
constexpr auto has_target(edge<node<F>, node<G>> const& e, node<H> const& n) {
  return hana::false_c;
}

template <typename F, typename G>
constexpr auto has_target(edge<node<F>, node<G>> const& e, node<G> const& n) {
  return hana::true_c;
}

template <typename F, typename ...G, typename H>
constexpr auto has_target(edge<node<F>, hana::tuple<node<G>...>> const& e, node<H> const& n) {
  return decltype(hana::contains(std::declval<hana::tuple<node<G>...>>(), n)){};
}

template <typename F, typename G, typename H>
constexpr auto has_source(edge<node<F>, node<G>> const& e, node<H> const& n) {
  return hana::false_c;
}

template <typename F, typename G>
constexpr auto has_source(edge<node<F>, node<G>> const& e, node<F> const& n) {
  return hana::true_c;
}

template <typename ...F, typename G, typename H>
constexpr auto has_source(edge<hana::tuple<node<F>...>, node<G>> const& e, node<H> const& n) {
  return decltype(hana::contains(std::declval<hana::tuple<node<F>...>>(), n)){};
}


template <typename F, typename G>
constexpr auto targets(edge<node<F>, node<G>> const& e) {
  return hana::tuple<node<G>>{};
}

template <typename F, typename ...G>
constexpr auto targets(edge<node<F>, hana::tuple<node<G>...>> const& e) {
  return hana::tuple<node<G>...>{};
}

template <typename ...F, typename G>
constexpr auto targets(edge<hana::tuple<node<F>...>, node<G>> const& e) {
  return hana::tuple<node<G>>{};
}

template <typename F, typename G>
constexpr auto sources(edge<node<F>, node<G>> const& e) {
  return hana::tuple<node<F>>{};
}

template <typename ...F, typename G>
constexpr auto sources(edge<hana::tuple<node<F>...>, node<G>> const& e) {
  return hana::tuple<node<F>...>{};
}

template <typename F, typename ...G>
constexpr auto sources(edge<node<F>, hana::tuple<node<G>...>> const& e) {
  return hana::tuple<node<F>>{};
}

//////////////////////////////////////////////////////////////////////////////
// Graph
//////////////////////////////////////////////////////////////////////////////
// Rules:
//  - Every node must appear as the target of exactly one edge

auto output = make_node([](auto const& result) {
  return result;
});

template <typename Nodes, typename Edges>
struct graph {
  constexpr explicit graph(Nodes const& nodes, Edges const& edges)
    : nodes_{nodes}, edges_{edges}
  { }

  Nodes nodes_;
  Edges edges_;


  template <typename F>
  auto producers(node<F> const& n) const {
    auto const& edge = *hana::find_if(this->edges_, [&n](auto const& e) {
      return hana::contains(hana::targets(e), n);
    });
    return sources(edge);
  }

  template <typename Node>
  auto evaluate(std::launch policy, Node const& node) const {
    auto future_args = hana::transform(producers(node), [&](auto const& f) {
      return evaluate(policy, f);
    });

    return hana::unpack(future_args, [&](auto const& ...args) {
      return std::async(policy, node, args.get()...);
    });
  }

  auto operator()(std::launch policy) const {
    return evaluate(policy, output);
  }

















  template <typename Nodes>
  auto linearized(Nodes const& nodes) const {
    return hana::sort(nodes, [&](auto const& f, auto const& g) {
      return has_dependency(g, f);
    });
  }

  template <typename Input>
  auto operator()(Input const& inp, std::launch policy) const {

    auto nodes = hana::to_tuple(gather_unique_nodes(edges_));
    auto linear = linearized(this->nodes_);
    auto groups = hana::group(linear, [&](auto const& a, auto const& b) {
      return independent(a, b);
    });

    hana::for_each(groups, [policy](auto const& group) {
      auto futures = hana::transform(group, [policy](auto const& f) {
        return std::async(policy, f);
      });
      hana::for_each(std::move(futures), [](auto const& f) {
        f.wait();
      });
    });
  }

  template <typename F>
  auto consumers(node<F> const& n) const {
    return hana::fold_left(this->edges_, hana::make_set(), [&](auto cons, auto e) {
      return hana::eval_if(hana::contains(sources(e), n),
        [&](auto _) { return hana::union_(cons, hana::to_set(targets(_(e)))); },
        [&]         { return cons; }
      );
    });
  }

  template <typename F>
  auto dependencies(node<F> const& n) const {
    return hana::fold_left(this->edges_, hana::make_set(), [&](auto deps, auto e) {
      return hana::eval_if(hana::contains(targets(e), n),
        [&](auto _) {
          auto recursive = hana::fold_left(sources(_(e)), deps, [&](auto deps, auto const& s) {
            return hana::eval_if(hana::contains(deps, s),
              [&] { return deps; },
              [&](auto _) {
                return hana::union_(hana::insert(deps, _(s)), dependencies(_(s)));
              }
            );
          });
          return recursive;
        },
        [&] { return deps; }
      );
    });
  }

  // Returns whether f >> g, i.e. whether g depends on f.
  template <typename F, typename G>
  auto has_dependency(node<F> const& f, node<G> const& g) const {
    return hana::contains(dependencies(g), f);
  }

  template <typename F, typename G>
  auto independent(node<F> const& f, node<G> const& g) const {
    return !has_dependency(f, g) && !has_dependency(g, f);
  }

  template <typename Fs>
  auto linearized(Fs const& fs) {
    return hana::sort(fs, [](auto const& f, auto const& g) {
      return depends_on(g, f);
    });
  }
};

template <typename Edges>
auto gather_unique_nodes(Edges const& edges) {
  return hana::fold_left(edges, hana::make_set(), [](auto nodes, auto e) {
    return hana::union_(nodes, hana::to_set(targets(e)));
  });
}

template <typename ...Edges>
constexpr auto make_graph(Edges const& ...e) {
  auto edges = hana::make_tuple(e...);
  auto nodes = hana::to_tuple(gather_unique_nodes(edges));
  return graph<decltype(nodes), decltype(edges)>{nodes, edges};
}

auto output = make_node([](auto result) { });

//////////////////////////////////////////////////////////////////////////////
// EDSL
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename G>
constexpr auto operator,(node<F> const& f, node<G> const& g) {
  return hana::make_tuple(f, g);
}

template <typename ...F, typename G>
constexpr auto operator,(hana::tuple<node<F>...> const& f, node<G> const& g) {
  return hana::append(f, g);
}

template <typename F, typename G>
constexpr auto operator>>(node<F> const& from, node<G> const& to) {
  return edge<node<F>, node<G>>{};
}

template <typename ...F, typename G>
constexpr auto operator>>(hana::tuple<node<F>...> const& from, node<G> const& to) {
  return edge<hana::tuple<node<F>...>, node<G>>{};
}

template <typename F, typename ...G>
constexpr auto operator>>(node<F> const& from, hana::tuple<node<G>...> const& to) {
  return edge<node<F>, hana::tuple<node<G>...>>{};
}

#endif // !HANA_GRAPH_HPP

#if 0

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

#ifdef CPP17
template <typename F, typename G>
auto depends_on(F const& f, G const& g) {
  if constexpr (hana::contains(f.dependencies, g)) {
    return hana::true_c;
  } else {
    return hana::any_of(f.dependencies, [&](auto const& dep) {
      return depends_on(dep, g);
    });
  }
}
#endif

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

template <typename F>
auto run_parallel(F const& f, std::launch policy) {
  auto fs = linearized(hana::prepend(all_dependencies(f), f));
  auto independent_groups = hana::group(fs, [](auto const& a, auto const& b) {
    return independent(a, b);
  });

  hana::for_each(independent_groups, [policy](auto const& group) {
    auto futures = hana::transform(group, [policy](auto const& f) {
      return std::async(policy, f);
    });
    hana::for_each(std::move(futures), [](auto const& f) {
      f.wait();
    });
  });
};

#endif
