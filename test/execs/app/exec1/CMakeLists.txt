add_executable(app_test_exec1)

target_sources(app_test_exec1 PRIVATE
    main.cpp
    value_holder.cpp
    current_temperature.cpp
    current_pressure.cpp
    current_humidity.cpp
    current_light_intensity.cpp
    temperature_requirement_list.cpp
    pressure_requirement_list.cpp
    humidity_requirement_list.cpp
    light_intensity_requirement_list.cpp

    mocks/mock_temperature_value.cpp
    mocks/mock_pressure_value.cpp
    mocks/mock_humidity_value.cpp
    mocks/mock_light_intensity_value.cpp
    mocks/mock_variable_requirement_list.cpp
)

target_link_libraries(app_test_exec1 PRIVATE test_common)

# functions that are mocked are defined twice - once in production code, once in the mock.
# -z muldefs flag tells the linker not to throw an error because of multiple definitions, but use the
# first definition. We add mocks to the app_test_exec1 target before linking against test_common which contains
# production code. This way, the linker will use the value_holder mocks instead of real value_holder.
target_link_options(app_test_exec1 PRIVATE -Wl,-z,muldefs)

# Register executable with test runner
add_test(NAME app_test_exec1 COMMAND app_test_exec1)
