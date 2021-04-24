cmake_minimum_required(VERSION 3.0)
project(MetaStuffTest)

find_package(nlohmann_json 3.2.0 REQUIRED)

set(exe_name meta-stuff-test)

add_executable(${exe_name})

target_sources(
    ${exe_name}
    PRIVATE
        main.cpp
        StringCast.cpp
        StringCast.h
        JsonCast.h
        MovieInfo.h
        Person.h
        JsonCast.inl)

target_link_libraries(
    ${exe_name}
    PRIVATE
        MetaStuff
        nlohmann_json::nlohmann_json)
