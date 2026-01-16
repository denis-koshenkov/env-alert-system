# Contains all chosen interface implementations for this build 
add_library(interfaces INTERFACE)

# Add interface definitions to include path
# Not adding "defs" directory to include path, because that directory defines interfaces for which
# the implementation needs to provide a header, and possibly a source file. If we add the defs directory
# to the include path, we include both the interface definition header and the implementation header,
# and that can get ugly because the same macros are defined in both files.
target_include_directories(interfaces INTERFACE defs/include)

# Select implementations based on the build type
if (BUILD_TESTS)
    add_subdirectory("implementations/eas_assert/cpputest")
    # Logging is not necessary when running unit tests
    add_subdirectory("implementations/eas_log/disabled")
    # Messages queues are not used when running unit tests
    add_subdirectory("implementations/osal/eas_message_queue/noop")
    # Threads are not used when running unit tests
    add_subdirectory("implementations/osal/eas_thread/noop")
    add_subdirectory("implementations/eas_current_time/fake")
    add_subdirectory("implementations/eas_timer/cppumock")
    add_subdirectory("implementations/led_notification_allocator/cppumock")
    add_subdirectory("implementations/variable_requirement_allocator/cppumock")
    add_subdirectory("implementations/linked_list_node_allocator/cppumock")
else()
    add_subdirectory("implementations/eas_assert/zephyr")
    add_subdirectory("implementations/eas_log/zephyr")
    add_subdirectory("implementations/osal/eas_message_queue/zephyr")
    add_subdirectory("implementations/osal/eas_thread/zephyr")
    add_subdirectory("implementations/eas_current_time/zephyr")
    add_subdirectory("implementations/eas_timer/zephyr")
    add_subdirectory("implementations/led_notification_allocator/zephyr_mem_blocks")
    add_subdirectory("implementations/variable_requirement_allocator/zephyr_mem_blocks")
    add_subdirectory("implementations/linked_list_node_allocator/zephyr_mem_blocks")
endif()
