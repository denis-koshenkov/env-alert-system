cmake_minimum_required(VERSION 4.0.3)

include(cmake/BuildOptions.cmake)

if(NOT BUILD_TESTS)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
endif()

project(env-alert-system)

if (BUILD_TESTS)
    # Needed so that asserts actually fire
    # TODO: to -DCMAKE_BUILD_TYPE=Debug in the west command implementation instead of this
    set(CMAKE_BUILD_TYPE "Debug")

    add_executable(app_tests)
    target_sources(app_tests PRIVATE test/main.cpp)

    # Application sources
    target_sources(app_tests PRIVATE src/temperature_value.c)

    # Include directories
    target_include_directories(app_tests PRIVATE src src/hal)
    target_include_directories(app_tests PRIVATE port/unit-test-desktop/include)
    
    # Test sources
    target_sources(app_tests PRIVATE test/temperature_value.cpp)

    # The policy allows us to change options of external projects using set()
    cmake_policy(SET CMP0077 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    set(TESTS OFF) # Disable cpputest self-tests
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../cpputest
        ${CMAKE_CURRENT_BINARY_DIR}/cpputest
    )

    target_link_libraries(app_tests PRIVATE
        CppUTest
        CppUTestExt)
else()
    target_sources(app PRIVATE src/main.c)
endif()
