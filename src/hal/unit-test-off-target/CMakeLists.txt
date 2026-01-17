add_library(hal INTERFACE)

set(VIRTUAL_DEVICES_DIR "${CMAKE_SOURCE_DIR}/src/virtual_devices")

target_sources(hal INTERFACE
    hw_platform.c
    transceiver.cpp

    # Virtual devices used by this hw_platform implementation
    ${VIRTUAL_DEVICES_DIR}/virtual_led_mock.cpp
)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VIRTUAL_DEVICES_DIR}
)
