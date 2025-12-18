target_sources(eas_app INTERFACE
    alert_adder.c
    alert_validator.c
    alert_remover.c
    central_event_queue.c
    eas_timer_callback_executor.c
    new_sample_callbacks.c
)

target_include_directories(eas_app INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(event_handlers)
