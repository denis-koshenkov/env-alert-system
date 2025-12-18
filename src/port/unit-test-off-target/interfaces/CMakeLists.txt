target_sources(port INTERFACE
    variable_requirement_allocator.cpp
    linked_list_node_allocator.cpp
    led_notification_allocator.cpp
    eas_timer.cpp
    virtual_led_mock.cpp
    eas_current_time.c
    transceiver.cpp
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(hal)
add_subdirectory(osal)
add_subdirectory(header)
