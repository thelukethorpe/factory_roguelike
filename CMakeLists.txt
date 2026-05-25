cmake_minimum_required(VERSION 3.16)

project(FactoryRoguelike VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.16
)

FetchContent_MakeAvailable(SDL3)

add_executable(game
    src/main.cpp
)

target_link_libraries(game PRIVATE SDL3::SDL3)
