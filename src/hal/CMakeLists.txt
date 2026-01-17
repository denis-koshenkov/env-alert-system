add_library(hal INTERFACE)

set(VIRTUAL_DEVICES_DIR "${CMAKE_SOURCE_DIR}/src/virtual_devices")

target_include_directories(hal INTERFACE
    def
    ${VIRTUAL_DEVICES_DIR}
)

add_subdirectory(impl/${PORT})
