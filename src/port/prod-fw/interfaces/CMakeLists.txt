target_sources(port INTERFACE
    linked_list_node_allocator.c
    variable_requirement_allocator.c
    led_notification_allocator.c
    eas_current_time.c
    eas_timer.c
)

add_subdirectory(hal)
add_subdirectory(osal)
