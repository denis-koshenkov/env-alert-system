target_sources(hal INTERFACE
    virtual_sht31.c
    virtual_bmp280.c
    virtual_bh1750.c
)

target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
