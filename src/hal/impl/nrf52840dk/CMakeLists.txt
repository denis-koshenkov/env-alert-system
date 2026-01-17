target_sources(hal INTERFACE
    hw_platform.c

    # Virtual devices used by this hw_platform implementation
    ${VIRTUAL_DEVICES_DIR}/virtual_bh1750.c
    ${VIRTUAL_DEVICES_DIR}/virtual_bmp280.c
    ${VIRTUAL_DEVICES_DIR}/virtual_sht31.c
    ${VIRTUAL_DEVICES_DIR}/virtual_led_nrf52840.c
    ${VIRTUAL_DEVICES_DIR}/virtual_transceiver_nrf_ble.c
)

# Add external device drivers to the build
include(${CMAKE_SOURCE_DIR}/cmake/sensirion_sht3x.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/rohm-bh1750fvi.cmake)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(drivers_glue)
