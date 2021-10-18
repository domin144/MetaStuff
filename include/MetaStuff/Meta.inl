#include <cassert>
#include <cstring>
#include <string_view>
#include <tuple>

#include "Member.h"
#include "detail/template_helpers.h"

namespace meta
{

template <typename... Args>
constexpr auto members(Args&&... args)
{
    // just this... but may become more complex later, who knows!
    //  Still, better no to expose too much to end-user.
    return std::make_tuple(std::forward<Args>(args)...);
}

template <typename Class>
constexpr auto registerMembers()
{
    return 0;
}

template <typename Class>
constexpr auto getName()
{
    return registerName<Class>();
}

template <typename Class>
constexpr std::size_t getMemberCount()
{
	return std::tuple_size<decltype(registerMembers<Class>())>::value;
}

template <typename Class>
constexpr auto getMembers()
{
    return registerMembers<Class>();
}

template <typename Class>
constexpr bool isRegistered()
{
    return std::is_class_v<decltype(getMembers<Class>())>;
}

namespace detail {
template<typename ...Members>
constexpr bool hasMemberInTuple(const std::string_view name, const std::tuple<Members...> &tuple)
{
    return std::apply(
        [&](const Members&... members) {
            return ((std::string_view(members.getName()) == name) || ...);
        },
        tuple);
}
}

template <typename Class>
constexpr bool hasMember(const std::string_view name)
{
    return detail::hasMemberInTuple(name, getMembers<Class>());
}

template <typename Class, typename F, typename>
void doForAllMembers(F&& f)
{
    detail::for_tuple(std::forward<F>(f), getMembers<Class>());
}

namespace detail {
template <typename Tuple, std::size_t... indices>
constexpr std::size_t getMemberIndexFromTuple(
    const std::string_view name,
    const Tuple& tuple,
    const std::index_sequence<indices...>)
{
	return (
		(std::string_view(std::get<indices>(tuple).getName()) == name ? indices
																	  : 0)
		+ ...);
}
}

template <typename Class>
constexpr std::size_t getMemberIndex(const std::string_view name)
{
	// static_assert (hasMember<Class>(name));
	return detail::getMemberIndexFromTuple(
		name,
		getMembers<Class>(),
		std::make_index_sequence<getMemberCount<Class>()>());
}

template <typename Class, typename T, typename F>
auto doForMember(const std::string_view name, F&& f)
{
    doForAllMembers<Class>(
            [&](const auto& member)
            {
                if (name == member.getName()) {
                    using MemberT = meta::get_member_type<decltype(member)>;
                    assert((std::is_same<MemberT, T>::value) && "Member doesn't have type T");
                    detail::call_if<std::is_same<MemberT, T>::value>(std::forward<F>(f), member);
                }
            }
        );
}

template <typename T, typename Class>
T& accessMember(Class& obj, const std::string_view name)
{
    T *result = nullptr;
    doForMember<Class, T>(name,
        [&obj, &result](const auto& member)
        {
            result = &member.access(obj);
        }
    );
    return *result;
}

template <typename T, typename Class>
const T& accessMember(const Class& obj, const std::string_view name)
{
    return doForMember<Class, T>(name,
        [&obj](const auto& member)
        {
            return member.access(obj);
        }
    );
}

template <typename T, typename Class>
T getMemberValue(Class& obj, const std::string_view name)
{
    return accessMember<T>(obj, name);
}

template <typename T, typename Class, typename V, typename>
void setMemberValue(Class& obj, const std::string_view name, V&& value)
{
    accessMember<T>(obj, name);
}

} // end of namespace meta
