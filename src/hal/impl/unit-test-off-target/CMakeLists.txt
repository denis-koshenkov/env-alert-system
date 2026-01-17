target_sources(hal INTERFACE
    hw_platform.c
    transceiver.cpp

    # Virtual devices used by this hw_platform implementation
    ${VIRTUAL_DEVICES_DIR}/virtual_led_mock.cpp
)

target_include_directories(hal INTERFACE
    # So that unit tests can include mock_transceiver.h
    ${CMAKE_CURRENT_SOURCE_DIR}
)
