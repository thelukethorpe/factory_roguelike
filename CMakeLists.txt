cmake_minimum_required(VERSION 3.16)

project(cpp_transport VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Match the current local development behavior without affecting consumers.
if(PROJECT_IS_TOP_LEVEL)
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
        add_compile_options(-O3)
    endif()
endif()

file(GLOB CPP_TRANSPORT_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(cpp_transport ${CPP_TRANSPORT_SOURCES})
add_library(cpp_transport::cpp_transport ALIAS cpp_transport)

target_include_directories(cpp_transport
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(cpp_transport PUBLIC cxx_std_20)

install(
    TARGETS cpp_transport
    EXPORT cpp_transportTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(
    EXPORT cpp_transportTargets
    NAMESPACE cpp_transport::
    FILE cpp_transportTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_transport
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpp_transportConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_transportConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_transport
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpp_transportConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cpp_transportConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cpp_transportConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpp_transport
)

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    set(DOCTEST_NO_INSTALL ON CACHE BOOL "Disable doctest install rules" FORCE)

    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.12
    )
    FetchContent_MakeAvailable(doctest)

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        tests/*.cpp
    )

    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests PRIVATE doctest cpp_transport)

    add_test(
        NAME all_test
        COMMAND tests
    )
endif()
