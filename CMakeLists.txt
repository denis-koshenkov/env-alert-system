cmake_minimum_required(VERSION 4.0.3)

include(cmake/BuildOptions.cmake)

if(NOT BUILD_TESTS)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
endif()

project(env-alert-system)

if (BUILD_TESTS)
    add_executable(app_tests)
    target_sources(app_tests PRIVATE test/main.cpp)
    target_sources(app_tests PRIVATE test/first_test.cpp)

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
