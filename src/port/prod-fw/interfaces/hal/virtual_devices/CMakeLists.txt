target_sources(port INTERFACE
    virtual_sht31.c
    virtual_bmp280.c
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
