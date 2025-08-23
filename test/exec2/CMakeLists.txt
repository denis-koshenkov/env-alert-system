add_executable(test_exec2)

target_sources(test_exec2 PRIVATE
    main.cpp
    temperature_value.cpp
    pressure_value.cpp
    humidity_value.cpp
    light_intensity_value.cpp
    variable_requirement.cpp
    temperature_requirement.cpp
    temperature_requirement.c
    pressure_requirement.cpp
    pressure_requirement.c
    humidity_requirement.cpp
    humidity_requirement.c
    light_intensity_requirement.cpp
    light_intensity_requirement.c

    mocks/mock_value_holder.cpp
    mocks/mock_variable_requirement_allocator.cpp
    mocks/mock_current_temperature.cpp
    mocks/mock_current_pressure.cpp
    mocks/mock_current_humidity.cpp
    mocks/mock_current_light_intensity.cpp

    mocks/variable_requirements/mock_variable_requirement.c
)

target_link_libraries(test_exec2 PRIVATE test_common)

# value_holder functions are defined twice - once in production code, once in the mock.
# -z muldefs flag tells the linker not to throw an error because of multiple definitions, but use the
# first definition. We add mocks to the test_exec2 target before linking against test_common which contains
# production code. This way, the linker will use the value_holder mocks instead of real value_holder.
target_link_options(test_exec2 PRIVATE -Wl,-z,muldefs)
