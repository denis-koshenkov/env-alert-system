add_executable(port_test_exec)

target_sources(port_test_exec PRIVATE
    main.cpp
    memory_block_allocator.cpp
)

target_link_libraries(port_test_exec PRIVATE test_common)
