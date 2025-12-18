add_library(port INTERFACE)

target_sources(port INTERFACE
    main.c
)

target_include_directories(port INTERFACE
    include
)

add_subdirectory(interfaces)

# app is the built-in Zephyr executable
target_link_libraries(app PRIVATE eas_app port)
