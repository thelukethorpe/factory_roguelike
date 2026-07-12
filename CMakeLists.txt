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
# spdlog
#

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(spdlog)

#
# Executables
#

add_executable(client
    src/app/client/client_main.cpp
)

add_executable(server
    src/app/server/server_main.cpp
)

#
# Core library
#

file(GLOB_RECURSE CORE_LIB_SOURCES CONFIGURE_DEPENDS
    src/core/*.cpp
)

add_library(core_lib
    ${CORE_LIB_SOURCES}
)

target_link_libraries(core_lib PUBLIC spdlog::spdlog)

target_include_directories(core_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#
# Client library (header-only)
#
add_library(client_lib INTERFACE)

target_link_libraries(client_lib
    INTERFACE
        core_lib
)

target_include_directories(client_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#
# Server library (header-only)
#
add_library(server_lib INTERFACE)

target_link_libraries(server_lib
    INTERFACE
        core_lib
)

target_include_directories(server_lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#
# Client app
#

file(GLOB_RECURSE CLIENT_APP_SOURCES CONFIGURE_DEPENDS
    src/app/client/*.cpp
)

add_library(client_app
    ${CLIENT_APP_SOURCES}
)

target_link_libraries(client_app PUBLIC client_lib SDL3::SDL3)


#
# Server app
#

file(GLOB_RECURSE SERVER_APP_SOURCES CONFIGURE_DEPENDS
    src/app/server/*.cpp
)

add_library(server_app
    ${SERVER_APP_SOURCES}
)

target_link_libraries(server_app PUBLIC server_lib)

#
# Link libraries to executables
#

target_link_libraries(client PRIVATE client_app)
target_link_libraries(server PRIVATE server_app)

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

target_link_libraries(tests PRIVATE doctest core_lib)

#
# Enable ctest
#

enable_testing()

add_test(
    NAME all_test
    COMMAND tests
)
