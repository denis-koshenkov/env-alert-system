add_library(port INTERFACE)

target_sources(port INTERFACE
    interfaces/variable_requirement_allocator.cpp
    interfaces/linked_list_node_allocator.cpp
    interfaces/eas_timer.cpp
    interfaces/led.cpp
)

target_include_directories(port INTERFACE
    include
    interfaces
)

add_subdirectory(test)
