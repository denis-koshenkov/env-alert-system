add_executable(app_test_exec2)

target_sources(app_test_exec2 PRIVATE
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
    variable_requirement_list.cpp
    variable_requirement_list.c
    linked_list.cpp
    alert_condition.cpp
    alert_raiser.cpp
    connectivity_notifier.cpp
    led_notifier.cpp
    led_manager.cpp
    eas_time.cpp
    alert_evaluation_readiness.cpp
    alert_validator.cpp
    alert_validator.c
    msg_transceiver.c
    msg_transceiver.cpp
    msg_transceiver_no_setup.c
    msg_transceiver_no_setup.cpp

    mocks/mock_value_holder.cpp
    mocks/mock_current_temperature.cpp
    mocks/mock_current_pressure.cpp
    mocks/mock_current_humidity.cpp
    mocks/mock_current_light_intensity.cpp
    mocks/mock_variable_requirement.c
    mocks/mock_alert_notifier.cpp
    mocks/mock_connectivity_notification_sender.cpp
    mocks/mock_led_notification_executor.cpp
)

target_link_libraries(app_test_exec2 PRIVATE test_common)

# value_holder functions are defined twice - once in production code, once in the mock.
# -z muldefs flag tells the linker not to throw an error because of multiple definitions, but use the
# first definition. We add mocks to the app_test_exec2 target before linking against test_common which contains
# production code. This way, the linker will use the value_holder mocks instead of real value_holder.
target_link_options(app_test_exec2 PRIVATE -Wl,-z,muldefs)

# Register executable with test runner
add_test(NAME app_test_exec2 COMMAND app_test_exec2)
