#include "JsonCast.h"
#include "Person.h"
#include <MetaStuff/Meta.h>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>

class Unregistered {};


void printSeparator()
{
    std::cout << "========================\n";
}

int main()
{
    Person person;
    person.age = 25;
    person.salary = 3.50f;
    person.name = "Alex"; // I'm a person!

//    person.favouriteMovies["Nostalgia Critic"] = { MovieInfo{ "The Room", 8.5f } };
//    person.favouriteMovies["John Tron"] = { MovieInfo{ "Goosebumps", 10.0f },
//        MovieInfo{ "Talking Cat", 9.0f } };

    // printing members of different classes
    std::cout << "Members of class Person:\n";
    meta::doForAllMembers<Person>(
        [](const auto& member)
    {
        std::cout << "* " << member.getName() << '\n';
    }
    );

    std::cout << "Members of class MovieInfo:\n";
    meta::doForAllMembers<MovieInfo>(
        [](const auto& member)
    {
        std::cout << "* " << member.getName() << '\n';
    }
    );

    printSeparator();

    // checking if classes are registered
    if (meta::isRegistered<Person>())
    {
        std::cout << "Person class is registered\n";
        std::cout << "It has " << meta::getMemberCount<Person>()
                  << " members registered.\n";
    }

	/* meta::isRegistered is constexpr, so can be used in enable_if and
	 * static_assert! */
	static_assert(
		meta::isRegistered<Person>(), "Person class is not registered!");
	static_assert(
		meta::getMemberCount<Person>() == 3,
		"Person does not have 3 members registered?");
	//	static_assert(
	//	    meta::getMemberCount<Person>() == 4,
	//	    "Person does not have 4 members registered?");

	// checking if class has a member
	static_assert(
		meta::hasMember<Person>("age"),
		"Person does not have a member named \"age\"");
	static_assert(
		!meta::hasMember<Person>("weight"),
		"Person does have a member named \"weight\"");

	static_assert(
		!meta::isRegistered<Unregistered>(), "Unregistered is registered");
	static_assert(
		meta::getMemberIndex<Person>("age") == 0,
		"Member \"age\" does not have index 0.");
	static_assert(
		std::get<0>(meta::getMembers<Person>()).getName() == "age",
		"First field is not named \"age\"");

	meta::doForMember<Person, int>("age", [](const auto &){});

    // getting members
    auto age = meta::accessMember<int>(person, "age");
    std::cout << "Got person's age: " << age << '\n';

    auto name = meta::accessMember<std::string>(person, "name");
    std::cout << "Got person's name: " << name << '\n';

    // setting members
    meta::accessMember<std::string>(person, "name") = "Ron Burgundy";
    name = meta::getMemberValue<std::string>(person, "name");
    std::cout << "Changed person's name to " << name << '\n';

    printSeparator();

    // And here's how you can serialize/deserialize
    // (if you write a function for your type)
    std::cout << "Serializing person:" << '\n';
    json root = person;
    std::cout << std::setw(4) << root << std::endl;

    //Unregistered y;
    //json root2 = y; // this will fail at compile time

    printSeparator();

    std::cout << "Serializing Person 2 from JSON:\n";

    auto person2 = root.get<Person>();
    std::cout << "Person 2 name is " << person2.getName() << " too!" << '\n';

#ifdef _WIN32 // okay, this is not cool code, sorry :D
    system("pause");
#endif
}
