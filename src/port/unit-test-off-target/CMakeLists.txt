add_library(port INTERFACE)

target_include_directories(port INTERFACE
    include
)

add_subdirectory(test)
