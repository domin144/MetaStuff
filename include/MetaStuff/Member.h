/* -----------------------------------------------------------------------------------------------

Member<Class, T> is a representation of a registered member
Class - a class this member belongs to
T - type of that member

Member contains pointer to data member which can be used to get / set stuff
Different getters and setters can be added via fluent interface (see addGetter/addConstGetter/addSetter)
Getters and setters are always preffered to using raw data member pointers (because why would you
set them in the other place?)

-------------------------------------------------------------------------------------------------*/

#ifndef METASTUFF_MEMBER_H
#define METASTUFF_MEMBER_H

#include <set>
#include <stdexcept>

namespace meta
{

template <typename Class, typename T>
using member_ptr_t = T Class::*;

// MemberType is Member<T, Class>
template <typename MemberType>
using get_member_type = typename std::decay_t<MemberType>::member_type;

template <typename Class, typename T>
struct Member
{
    using class_type = Class;
    using member_type = T;

private:
	std::string_view m_name;
	member_ptr_t<Class, T> m_ptr;

public:
    constexpr Member(
        const char* name, member_ptr_t<Class, T> member_pointer) :
        m_name {name}, m_ptr {member_pointer}
    {
    }

	constexpr std::string_view getName() const { return m_name; }
	T& access(class_type& object) const { return object.*m_ptr; }
	const T& access(const class_type& object) const { return object.*m_ptr; }
};

// useful function similar to make_pair which is used so you don't have to write this:
// member_t<SomeClass, int>("someName", &SomeClass::someInt); and can just to this:
// member("someName", &SomeClass::someInt);

template <typename Class, typename T>
constexpr Member<Class, T> member(
    const char* name, member_ptr_t<Class, T> member_ptr)
{
    return Member<Class, T> {name, member_ptr};
}

} // end of namespace meta

#endif /* METASTUFF_MEMBER_H */
