#include <tuple>

namespace meta {
namespace detail {

template <typename F, typename ...Members>
void for_tuple(F&& f, const std::tuple<Members...>& tuple)
{
    std::apply([&f](const Members&... members) { (f(members), ...); }, tuple);
}

template <bool Test,
    typename F, typename... Args,
    typename>
void call_if(F&& f, Args&&... args)
{
    f(std::forward<Args>(args)...);
}

template <bool Test,
    typename F, typename... Args,
    typename, typename>
void call_if(F&& /* f */, Args&&... /* args */)
{ /* do nothing */ }

} // end of namespace detail
} // end of namespace meta
