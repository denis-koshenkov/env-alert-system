add_library(test_common INTERFACE)

set(TESTS OFF) # Disable cpputest self-tests
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cpputest
    ${CMAKE_CURRENT_BINARY_DIR}/cpputest
)

target_link_libraries(test_common INTERFACE
    eas
    port
    test_internal
    CppUTest
    CppUTestExt
)

# Internal test helper modules
add_subdirectory(internal)

# Test executables of production code
add_subdirectory(execs/app/exec1)
add_subdirectory(execs/app/exec2)

# Test executables of internal test helper modules
add_subdirectory(execs/internal)
