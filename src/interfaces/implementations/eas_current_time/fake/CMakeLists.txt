target_sources(interfaces INTERFACE
    eas_current_time.c
)

target_include_directories(interfaces INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
