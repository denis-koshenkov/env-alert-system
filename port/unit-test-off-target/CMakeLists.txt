add_library(port INTERFACE)

target_sources(port INTERFACE
    memory_block_allocator.c
    linked_list_node_allocator.c
)

target_include_directories(port INTERFACE include)
