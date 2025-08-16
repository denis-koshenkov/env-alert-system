# This library contains a part of zephyr that is needed to build the unit testing application. For example, we want to use
# Zephyr's memory blocks allocator in our unit tests. We do not want to link the whole zephyr, because that would unnecessarily bloat our unit test build.
add_library(test_zephyr_minimal INTERFACE)

set(ZEPHYR_DIR ${CMAKE_SOURCE_DIR}/../nrf-sdk/zephyr)

target_sources(test_zephyr_minimal INTERFACE
    ${ZEPHYR_DIR}/lib/mem_blocks/mem_blocks.c
)

target_include_directories(test_zephyr_minimal INTERFACE
    ${ZEPHYR_DIR}/include/zephyr/sys
)
