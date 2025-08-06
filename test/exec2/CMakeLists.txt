add_executable(test_exec2)

target_sources(test_exec2 PRIVATE
    main.cpp
    temperature_value.cpp
    pressure_value.cpp

    mocks/mock_value_holder.cpp
)

target_link_libraries(test_exec2 PRIVATE test_common)

# value_holder functions are defined twice - once in production code, once in the mock.
# -z muldefs flag tells the linker not to throw an error because of multiple definitions, but use the
# first definition. We add mocks to the test_exec2 target before linking against test_common which contains
# production code. This way, the linker will use the value_holder mocks instead of real value_holder.
target_link_options(test_exec2 PRIVATE -Wl,-z,muldefs)
