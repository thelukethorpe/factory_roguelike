cmake_minimum_required(VERSION 3.16)

project(FactoryRoguelike VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

target_link_libraries(game PRIVATE SDL3::SDL3)


#
# Collect all tests files
#

file(GLOB TEST_SOURCES
    tests/*.cpp
)

#
# Test executable
#

add_executable(tests
    ${TEST_SOURCES}
)

target_link_libraries(tests PRIVATE doctest)

#
# Enable ctest
#

enable_testing()

add_test(
    NAME all_test
    COMMAND tests
)