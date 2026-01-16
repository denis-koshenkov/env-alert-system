target_sources(port INTERFACE
    linked_list_node_allocator.cpp
    virtual_led_mock.cpp
    transceiver.cpp
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
