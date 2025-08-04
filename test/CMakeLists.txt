add_library(test_common INTERFACE)

target_include_directories(test_common INTERFACE ${CMAKE_SOURCE_DIR}/port/unit-test-off-target/include)

set(TESTS OFF) # Disable cpputest self-tests
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cpputest
    ${CMAKE_CURRENT_BINARY_DIR}/cpputest
)

target_link_libraries(test_common INTERFACE
    eas
    CppUTest
    CppUTestExt
)

add_subdirectory(exec1)
add_subdirectory(exec2)

# TODO: register all unit test executables with CTest
