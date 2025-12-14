add_library(port INTERFACE)

target_sources(port INTERFACE
    interfaces/variable_requirement_allocator.cpp
    interfaces/linked_list_node_allocator.cpp
    interfaces/led_notification_allocator.cpp
    interfaces/eas_timer.cpp
    interfaces/hw_platform.c
    interfaces/virtual_led_mock.cpp
    interfaces/eas_current_time.c
    interfaces/transceiver.cpp
    interfaces/osal/eas_thread.c
    interfaces/osal/eas_message_queue.c
)

target_include_directories(port INTERFACE
    include
    interfaces
)

add_subdirectory(test)
