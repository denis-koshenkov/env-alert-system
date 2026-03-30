target_sources(hal INTERFACE
    eass.c
)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
