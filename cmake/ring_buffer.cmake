add_library(ring_buffer INTERFACE)

# Convert to absoluate path to remove /../../.. from the path
cmake_path(SET RING_BUFFER_SRC_DIR NORMALIZE "${CMAKE_SOURCE_DIR}/../ring-buffer/src")
target_sources(ring_buffer INTERFACE
    "${RING_BUFFER_SRC_DIR}/ring_buf.c"
)

target_include_directories(ring_buffer INTERFACE
    "${RING_BUFFER_SRC_DIR}"
)
