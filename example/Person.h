#pragma once

#include "MovieInfo.h"
#include <MetaStuff/Meta.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Person
{
    const std::string& getName() const
    {
        std::cout << "Got name with getter!\n";
        return name;
    }

    int age;
    std::string name;
    float salary;
//    std::unordered_map<std::string, std::vector<MovieInfo>> favouriteMovies;
};

namespace meta {

template <>
constexpr inline auto registerMembers<Person>()
{
    using T = Person;
    return members(
        member("age", &T::age), // access through getter/setter only!
        member("name", &T::name), // same, but ref getter/setter
        member("salary", &T::salary)/*,
        member("favouriteMovies", &T::favouriteMovies)*/);
}

} // end of namespace meta
