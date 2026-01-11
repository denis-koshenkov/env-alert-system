add_library(eas_app INTERFACE)

target_sources(eas_app INTERFACE
    value_holder.c
    temperature_value.c
    pressure_value.c
    humidity_value.c
    light_intensity_value.c
    current_temperature.c
    current_pressure.c
    current_humidity.c
    current_light_intensity.c
    temperature_requirement.c
    pressure_requirement.c
    humidity_requirement.c
    light_intensity_requirement.c
    variable_requirement.c
    variable_requirement_list.c
    temperature_requirement_list.c
    pressure_requirement_list.c
    humidity_requirement_list.c
    light_intensity_requirement_list.c
    alert_condition.c
    variable_requirement_evaluator.c
    alert_conditions.c
    alert_raiser.c
    alert_notifier.c
    alert_raisers.c
    connectivity_notifier.c
    connectivity_notification_sender.c
    led_notifier.c
    led_notification_executor.c
    led_manager.c
    alert_evaluation_readiness.c
)

target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(utils)
add_subdirectory(middleware)
add_subdirectory(glue)
add_subdirectory(interfaces)
