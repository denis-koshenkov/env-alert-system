add_library(utils INTERFACE)

target_sources(utils INTERFACE
    ops_queue.c
)

target_include_directories(utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
