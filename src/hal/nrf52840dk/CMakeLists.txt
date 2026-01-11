add_library(hal INTERFACE)

target_sources(hal INTERFACE
    hw_platform.c
    led_nrf52840.c
    transceiver.c
)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(virtual_devices)
add_subdirectory(drivers_glue)
