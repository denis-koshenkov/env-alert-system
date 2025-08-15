add_library(eas INTERFACE)

# main.c is not included, so that another main can be used for test executables
target_sources(eas INTERFACE
    value_holder.c
    temperature_value.c
    pressure_value.c
    humidity_value.c
    light_intensity_value.c
    variable_requirement.c
    temperature_requirement.c
)

target_include_directories(eas INTERFACE
    .
)
