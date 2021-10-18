#pragma once

#include "JsonCast.h"

template <typename T>
void to_json(json& j, const T& obj)
{
    j = meta::serialize(obj);
}

template <typename T>
void from_json(const json& j, T& obj)
{
    meta::deserialize(obj, j);
}

namespace meta
{

/////////////////// SERIALIZATION

template <typename Class,
    typename>
json serialize(const Class& obj)
{
    json value;
    meta::doForAllMembers<Class>(
        [&obj, &value](auto& member)
        {
            value[std::string{member.getName()}] = member.access(obj);
        }
    );
    return value;
}

template <typename Class,
    typename, typename>
json serialize(const Class& obj)
{
    return serialize_basic(obj);
}

template <typename Class>
json serialize_basic(const Class& obj)
{
    return json(obj);
}

// specialization for std::vector
template <typename T>
json serialize_basic(const std::vector<T>& obj)
{
    json value;
    int i = 0;
    for (auto& elem : obj) {
        value[i] = elem;
        ++i;
    }
    return value;
}

// specialization for std::unordered_map
template <typename K, typename V>
json serialize_basic(const std::unordered_map<K, V>& obj)
{
    json value;
    for (auto& pair : obj) {
        value.emplace(castToString(pair.first), pair.second);
    }
    return value;
}

/////////////////// DESERIALIZATION

template <typename Class>
Class deserialize(const json& obj)
{
    Class c;
    deserialize(c, obj);
    return c;
}

template <typename Class,
    typename>
void deserialize(Class& obj, const json& object)
{
    if (object.is_object())
    {
        meta::doForAllMembers<Class>([&obj, &object](auto& member) {
            auto& objectMember = object[std::string{member.getName()}];
            if (!objectMember.is_null())
            {
                member.access(obj) = objectMember;
            }
        });
    }
    else
    {
        throw std::runtime_error(
            "Error: can't deserialize from Json::json to Class.");
    }
}

template <typename Class,
    typename, typename>
void deserialize(Class& obj, const json& object)
{
    obj = object.get<Class>();
}

// specialization for std::vector
template <typename T>
void deserialize(std::vector<T>& obj, const json& object)
{
    obj.reserve(object.size()); // vector.resize() works only for default constructible types
    for (auto& elem : object) {
        obj.push_back(elem); // push rvalue
    }
}

// specialization for std::unodered_map
template <typename K, typename V>
void deserialize(std::unordered_map<K, V>& obj, const json& object)
{
    for (auto it = object.begin(); it != object.end(); ++it) {
        obj.emplace(fromString<K>(it.key()), it.value());
    }
}

}
