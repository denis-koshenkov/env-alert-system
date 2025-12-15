target_sources(eas_app INTERFACE
    new_sample_handler.c
    init_handler.c
)

target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
