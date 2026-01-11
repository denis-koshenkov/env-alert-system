add_library(port INTERFACE)

target_sources(port INTERFACE
    main.c
)

target_include_directories(port INTERFACE
    include
)

add_subdirectory(interfaces)
include(sensirion_sht3x.cmake)
include(rohm-bh1750fvi.cmake)

# app is the built-in Zephyr executable
target_link_libraries(app PRIVATE eas_app hal port utils sht3x_driver bh1750_driver)
