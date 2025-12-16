add_library(port INTERFACE)

target_sources(port INTERFACE
    main.c
    interfaces/linked_list_node_allocator.c
    interfaces/variable_requirement_allocator.c
    interfaces/led_notification_allocator.c
    interfaces/eas_current_time.c
    interfaces/osal/eas_thread.c
    interfaces/osal/eas_message_queue.c
)

target_include_directories(port INTERFACE
    include
    interfaces
)

# app is the built-in Zephyr executable
target_link_libraries(app PRIVATE eas_app port)
