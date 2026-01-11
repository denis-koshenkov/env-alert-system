add_library(hal INTERFACE)

target_sources(hal INTERFACE
    hw_platform.c
)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
