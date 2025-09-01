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
else()
    # Building production firmware
    set(PORT_DIR "prod-fw")
    target_sources(app PRIVATE src/main.c)
    target_link_libraries(app PRIVATE eas port)
endif()

add_subdirectory(src)
add_subdirectory("port/${PORT_DIR}")
