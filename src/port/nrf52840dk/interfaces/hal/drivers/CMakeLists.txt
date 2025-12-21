target_sources(port INTERFACE
    sht31.c
)

target_include_directories(port INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
