add_library(test_internal INTERFACE)

target_sources(test_internal INTERFACE
    fake_linked_list_node_allocator.c
    memory_block_allocator.c
)

target_include_directories(test_internal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
