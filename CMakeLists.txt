cmake_minimum_required(VERSION 4.0.3)

# find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(env-alert-system)

# add_executable(app)
# target_sources(app PRIVATE src/main.c)

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
