add_library(port INTERFACE)

target_sources(port INTERFACE
    interfaces/linked_list_node_allocator.c
    interfaces/variable_requirement_allocator.c
)

target_include_directories(port INTERFACE
    include
    interfaces
)
