add_library(bh1750_driver INTERFACE)

# Convert to absoluate path to remove /../../.. from the path
cmake_path(SET ROHM_BH1750FVI_SRC_DIR NORMALIZE "${CMAKE_SOURCE_DIR}/../rohm-bh1750fvi/src")
target_sources(bh1750_driver INTERFACE
    "${ROHM_BH1750FVI_SRC_DIR}/bh1750.c"
)

target_include_directories(bh1750_driver INTERFACE
    "${ROHM_BH1750FVI_SRC_DIR}"
)
