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

// reference getter/setter func pointer type
template <typename Class, typename T>
using ref_getter_func_ptr_t = const T& (Class::*)() const;

template <typename Class, typename T>
using ref_setter_func_ptr_t = void (Class::*)(const T&);

// value getter/setter func pointer type
template <typename Class, typename T>
using val_getter_func_ptr_t = T(Class::*)() const;

template <typename Class, typename T>
using val_setter_func_ptr_t = void (Class::*)(T);

// non const reference getter
template <typename Class, typename T>
using nonconst_ref_getter_func_ptr_t = T& (Class::*)();

// MemberType is Member<T, Class>
template <typename MemberType>
using get_member_type = typename std::decay_t<MemberType>::member_type;

template <typename Class, typename T>
struct Member
{
    using class_type = Class;//template <typename Class, typename T>
    //using member_get_ptr_t = T Class::*;

    //template <typename Class, typename T>
    //using member_set_ptr_t = T Class::*;
    using member_type = T;

private:
	std::string_view m_name;
	member_ptr_t<Class, T> m_ptr;

public:
	constexpr Member(
		const std::string_view name, member_ptr_t<Class, T> member_pointer) :
		m_name {name}, m_ptr {member_pointer}
	{
	}

	constexpr std::string_view getName() const { return m_name; }
	const T& access(const class_type& object) const { return object.*m_ptr; }
	T& access(class_type& object) const { return object.*m_ptr; }
	const T& get(const class_type& object) const { return access(object); }
	void set(class_type& object, const T& value) const
	{
		access(object) = value;
	}
};

template<typename Class, typename T>
struct ValGetSetMember
{
    using class_type = Class;
    using member_type = T;

private:
	std::string_view m_name;
	val_getter_func_ptr_t<Class, T> m_getter;
	val_setter_func_ptr_t<Class, T> m_setter;

public:
	constexpr ValGetSetMember(
		const char* name,
		val_getter_func_ptr_t<Class, T> getter,
		val_setter_func_ptr_t<Class, T> setter) :
		m_name {name}, m_getter {getter}, m_setter {setter}
	{
	}

	constexpr std::string_view getName() const { return m_name; }
	T get(const class_type& object) const { return (object.*m_getter)(); }
	void set(class_type& object, const T value) const
	{
		(object.*m_setter)(value);
	}
};

template<typename Class, typename T>
struct RefGetSetMember
{
    using class_type = Class;
    using member_type = T;

private:
	std::string_view m_name;
	ref_getter_func_ptr_t<Class, T> m_getter;
	ref_setter_func_ptr_t<Class, T> m_setter;

public:
	constexpr RefGetSetMember(
		const char* name,
		ref_getter_func_ptr_t<Class, T> getter,
		ref_setter_func_ptr_t<Class, T> setter) :
		m_name {name}, m_getter {getter}, m_setter {setter}
	{
	}

	constexpr std::string_view getName() const { return m_name; }
	const T& get(const class_type& object) const
	{
		return (object.*m_getter)();
	}
	void set(class_type& object, const T& value) const
	{
		(object.*m_setter)(value);
	}
};

template<typename Class, typename T>
struct NcRefGetMember
{
    using class_type = Class;
    using member_type = T;

private:
	std::string_view m_name;
	ref_getter_func_ptr_t<Class, T> m_getter;
	nonconst_ref_getter_func_ptr_t<Class, T> m_ncGetter;

public:
	constexpr NcRefGetMember(
		const char* name,
		ref_getter_func_ptr_t<Class, T> getter,
		nonconst_ref_getter_func_ptr_t<Class, T> ncGetter) :
		m_name {name}, m_getter {getter}, m_ncGetter {ncGetter}
	{
	}

	constexpr std::string_view getName() const { return m_name; }
	const T& access(const class_type& object) const { (object.*m_getter)(); }
	T& access(class_type& object) const { return (object.*m_ncGetter)(); }
	const T& get(const class_type& object) const { return access(object); }
	void set(class_type& object, const T& value) const
	{
		access(object) = value;
	}
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

template <typename Class, typename T>
constexpr ValGetSetMember<Class, T> member(
    const char* name,
    val_getter_func_ptr_t<Class, T> getter,
    val_setter_func_ptr_t<Class, T> setter)
{
    return ValGetSetMember<Class, T> {name, getter, setter};
}

template <typename Class, typename T>
constexpr RefGetSetMember<Class, T> member(
    const char* name,
    ref_getter_func_ptr_t<Class, T> getter,
    ref_setter_func_ptr_t<Class, T> setter)
{
    return RefGetSetMember<Class, T> {name, getter, setter};
}

template <typename Class, typename T>
constexpr NcRefGetMember<Class, T> member(
    const char* name,
    ref_getter_func_ptr_t<Class, T> getter,
    nonconst_ref_getter_func_ptr_t<Class, T> ncGetter)
{
    return NcRefGetMember<Class, T> {name, getter, ncGetter};
}

} // end of namespace meta

#endif /* METASTUFF_MEMBER_H */
