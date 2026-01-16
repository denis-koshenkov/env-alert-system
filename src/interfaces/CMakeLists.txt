# Contains all chosen interface implementations for this build 
add_library(interfaces INTERFACE)

# Add interface definitions to include path
target_include_directories(interfaces INTERFACE defs/include)

# Select implementations based on the build type
if (BUILD_TESTS)
    add_subdirectory("implementations/eas_assert/cpputest")
    # Logging is not necessary when running unit tests
    add_subdirectory("implementations/eas_log/disabled")
    # Messages queues are not used when running unit tests
    add_subdirectory("implementations/osal/eas_message_queue/noop")
else()
    add_subdirectory("implementations/eas_assert/zephyr")
    add_subdirectory("implementations/eas_log/zephyr")
    add_subdirectory("implementations/osal/eas_message_queue/zephyr")
endif()
