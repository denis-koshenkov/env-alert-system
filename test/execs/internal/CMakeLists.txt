add_executable(internal_test_exec)

target_sources(internal_test_exec PRIVATE
    main.cpp
    memory_block_allocator.cpp
)

# FIXME: internal_test_exec includes exec2 source
target_sources(internal_test_exec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../app/exec2/mocks/eas_timer.cpp
)

# FIXME: internal_test_exec includes exec2 directory
target_include_directories(internal_test_exec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../app/exec2/mocks
)

target_link_libraries(internal_test_exec PRIVATE test_common)
