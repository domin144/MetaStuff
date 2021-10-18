cmake_minimum_required(VERSION 3.0)
project(MetaStuff VERSION 0.0.2 LANGUAGES CXX)

cmake_policy(SET CMP0076 NEW)

include(GNUInstallDirs)

add_library(MetaStuff INTERFACE)
target_sources(
    MetaStuff
    INTERFACE
        $<BUILD_INTERFACE:
            ${CMAKE_CURRENT_SOURCE_DIR}/include/MetaStuff/detail/template_helpers.inl
            ${CMAKE_CURRENT_SOURCE_DIR}/include/MetaStuff/detail/template_helpers.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/MetaStuff/Member.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/MetaStuff/Meta.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/MetaStuff/Meta.inl>)
target_include_directories(
    MetaStuff
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/MetaStuff>)

# build examples
option(WITH_EXAMPLES "Build examples" OFF)
if(WITH_EXAMPLES)
    add_subdirectory(example)
endif()

# install step

# Install a pkg-config file, so other tools can find this.
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/MetaStuff
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    TARGETS MetaStuff
    EXPORT MetaStuffTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT MetaStuffTargets
    FILE MetaStuffTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/MetaStuff/cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/MetaStuffConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MetaStuffConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/MetaStuff/cmake)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MetaStuffConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
    ARCH_INDEPENDENT)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/MetaStuffConfig.cmake
    FILES ${CMAKE_CURRENT_BINARY_DIR}/MetaStuffConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/MetaStuff/cmake)
