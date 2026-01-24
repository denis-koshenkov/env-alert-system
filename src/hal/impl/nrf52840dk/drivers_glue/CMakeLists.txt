# So that includes are more descriptive, e.g. #include "sht3x/get_instance_memory.h" instead of just #include "get_instance_memory.h"
target_include_directories(hal INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(sht3x)
add_subdirectory(bh1750)
