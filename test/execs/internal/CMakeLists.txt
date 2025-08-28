add_executable(internal_test_exec)

target_sources(internal_test_exec PRIVATE
    main.cpp
    memory_block_allocator.cpp
)

target_link_libraries(internal_test_exec PRIVATE test_common)
