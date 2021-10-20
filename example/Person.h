#pragma once

#include "MovieInfo.h"
#include <MetaStuff/Meta.h>
#include <iostream>
#include <string>
#include <unordered_map>

struct Person
{
    void setAge(int a)
    {
        //std::cout << "Age is set by calling setter!\n";
        if (a >= 0 && a < 128) { // sorry, if you're older than 128
            age = a;
        } else {
            std::cout << "Can't set age. " << a << " is out of allowed range\n";
        }
    }

    int getAge() const {
        std::cout << "Got age with getter!\n";
        return age;
    }

    void setName(const std::string& name)
    {
        //std::cout << "Name is set by calling setter!\n";
        this->name = name;
    }

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
        member("age", &T::getAge, &T::setAge), // access through getter/setter only!
        member("name", &T::getName, &T::setName), // same, but ref getter/setter
        member("salary", &T::salary)/*,
        member("favouriteMovies", &T::favouriteMovies)*/);
}

} // end of namespace meta
