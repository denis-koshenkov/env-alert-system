target_sources(eas_app INTERFACE
    alert_adder.c
)

target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
