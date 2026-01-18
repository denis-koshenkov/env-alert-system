target_sources(interfaces INTERFACE
    eas_ring_buf.c
    ring_buf_get_inst_buf.c
)

# Include external ring buffer implementation into the build
include(${CMAKE_SOURCE_DIR}/cmake/ring_buffer.cmake)

target_link_libraries(interfaces INTERFACE ring_buffer)
