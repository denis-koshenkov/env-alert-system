add_library(eas INTERFACE)

# main.c is not included, so that another main can be used for test executables
target_sources(eas INTERFACE
    temperature_value.c
    value_holder.c
)

target_include_directories(eas INTERFACE
    .
    hal
)
