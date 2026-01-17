add_library(sht3x_driver INTERFACE)

# Convert to absoluate path to remove /../../.. from the path
cmake_path(SET SENSIRION_SHT3X_SRC_DIR NORMALIZE "${CMAKE_SOURCE_DIR}/../sensirion-sht3x/src")
target_sources(sht3x_driver INTERFACE
    "${SENSIRION_SHT3X_SRC_DIR}/sht3x.c"
)

target_include_directories(sht3x_driver INTERFACE
    "${SENSIRION_SHT3X_SRC_DIR}"
)
