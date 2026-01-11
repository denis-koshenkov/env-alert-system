cmake_minimum_required(VERSION 4.0.3)

include(cmake/BuildOptions.cmake)

if(NOT BUILD_TESTS)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
endif()

project(env-alert-system)

if(BUILD_TESTS)
    include(CTest)
endif()

# The policy allows us to change options of external projects using set()
# Needed for CppUTest
cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

if(BUILD_TESTS)
    set(PORT_DIR "unit-test-off-target")
    set(HAL_DIR "unit-test-off-target")
else()
    set(PORT_DIR "nrf52840dk")
    set(HAL_DIR "nrf52840dk")
endif()

add_subdirectory(src/app)
add_subdirectory("src/port/${PORT_DIR}")
add_subdirectory("src/hal/${HAL_DIR}")
add_subdirectory(src/utils)
