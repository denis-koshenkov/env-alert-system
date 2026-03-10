add_library(bmp280_driver INTERFACE)

# Convert to absoluate path to remove /../../.. from the path
cmake_path(SET BOSCH_BMP280_SRC_DIR NORMALIZE "${CMAKE_SOURCE_DIR}/../bosch-bmp280/src")
target_sources(bmp280_driver INTERFACE
    "${BOSCH_BMP280_SRC_DIR}/bmp280.c"
)

target_include_directories(bmp280_driver INTERFACE
    "${BOSCH_BMP280_SRC_DIR}"
)
