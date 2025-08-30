add_library(eas INTERFACE)

# main.c is not included, so that another main can be used for test executables
target_sources(eas INTERFACE
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
    variable_requirement_allocator.c
    linked_list_node_allocator.c
    variable_requirement_list.c
    temperature_requirement_list.c
    pressure_requirement_list.c
    humidity_requirement_list.c
    light_intensity_requirement_list.c
    alert_condition.c
)

target_include_directories(eas INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(utils)
