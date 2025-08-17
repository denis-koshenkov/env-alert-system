add_executable(test_exec1)

target_sources(test_exec1 PRIVATE
    main.cpp
    value_holder.cpp
    current_temperature.cpp

    mocks/mock_temperature_value.cpp
)

target_link_libraries(test_exec1 PRIVATE test_common)

# functions that are mocked are defined twice - once in production code, once in the mock.
# -z muldefs flag tells the linker not to throw an error because of multiple definitions, but use the
# first definition. We add mocks to the test_exec1 target before linking against test_common which contains
# production code. This way, the linker will use the value_holder mocks instead of real value_holder.
target_link_options(test_exec1 PRIVATE -Wl,-z,muldefs)
