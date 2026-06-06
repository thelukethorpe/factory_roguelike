cmake_minimum_required(VERSION 3.16)

project(FactoryRoguelike VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")

    add_compile_options(
        -g
        -O0
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )
    
    add_link_options(
        -fsanitize=address
        -fsanitize=undefined
    )

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")

    message(STATUS "Release build")

    add_compile_options(
        -O3
    )

endif()

include(FetchContent)

#
# SDL3
#

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.16
)

FetchContent_MakeAvailable(SDL3)

#
# doctest
#

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
)

FetchContent_MakeAvailable(doctest)

#
# Main game executable
#

add_executable(game
    src/main.cpp
)

file(GLOB_RECURSE TRANSPORT_LIB_SOURCES CONFIGURE_DEPENDS
    src/core/transport/*.cpp
)

add_library(transport_lib
    ${TRANSPORT_LIB_SOURCES}
)

target_include_directories(transport_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_link_libraries(game PRIVATE SDL3::SDL3 transport_lib)


#
# Collect all tests files
#

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    tests/*.cpp
)

#
# Test executable
#

add_executable(tests
    ${TEST_SOURCES}
)

target_link_libraries(tests PRIVATE doctest transport_lib)

#
# Enable ctest
#

enable_testing()

add_test(
    NAME all_test
    COMMAND tests
)