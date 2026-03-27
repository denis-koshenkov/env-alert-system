target_sources(hal INTERFACE
    hw_platform.c

    # Virtual devices used by this hw_platform implementation
    ${VIRTUAL_DEVICES_DIR}/virtual_transceiver_mock.cpp
)
