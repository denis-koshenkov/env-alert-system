target_sources(port INTERFACE
    eas_log.c
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
