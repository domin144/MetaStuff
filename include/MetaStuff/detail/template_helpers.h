// Various useful template stuff
// thanks to Vittorio Romeo and other people who came up with this stuff

#include <utility>

#pragma once

namespace meta {
namespace detail {

// for_each_arg - call f for each element from tuple
template <typename F, typename ...Members>
void for_tuple(F&& f, const std::tuple<Members...>& tuple);

// calls F if condition is true
// this is useful for templated lambdas, because they won't be
// instantiated with unneeded types
template <bool Test,
    typename F, typename... Args,
    typename = std::enable_if_t<Test>>
void call_if(F&& f, Args&&... args);

// calls F if condition is false
template <bool Test,
    typename F, typename... Args,
    typename = std::enable_if_t<!Test>,
    typename = void> // dummy type for difference between two functions
void call_if(F&& f, Args&&... args);

} // end of namespace detail
} // end of namespace meta

#include "template_helpers.inl"
