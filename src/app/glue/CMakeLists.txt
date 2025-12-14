target_sources(eas_app INTERFACE
    alert_adder.c
    alert_validator.c
    alert_remover.c
    new_sample_handler.c
    central_event_queue.c
)

target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
