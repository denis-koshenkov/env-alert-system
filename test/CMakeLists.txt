add_library(test_common INTERFACE)

target_include_directories(test_common INTERFACE ${CMAKE_SOURCE_DIR}/port/unit-test-off-target/include)

set(TESTS OFF) # Disable cpputest self-tests
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cpputest
    ${CMAKE_CURRENT_BINARY_DIR}/cpputest
)

target_link_libraries(test_common INTERFACE
    eas
    port
    CppUTest
    CppUTestExt
)

add_subdirectory(execs/app/exec1)
add_subdirectory(execs/app/exec2)
add_subdirectory(execs/port)

# Register all unit test executables with CTest
add_test(NAME app_test_exec1 COMMAND app_test_exec1)
add_test(NAME app_test_exec2 COMMAND app_test_exec2)
add_test(NAME port_test_exec COMMAND port_test_exec)
