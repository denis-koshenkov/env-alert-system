target_sources(port INTERFACE
    eas_current_time.c
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
