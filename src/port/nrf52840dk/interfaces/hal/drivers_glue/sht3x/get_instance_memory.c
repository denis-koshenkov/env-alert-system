#include <stdbool.h>

#include "get_instance_memory.h"
#include "sht3x_private.h"
#include "eas_assert.h"

void *sht3x_driver_get_instance_memory(void *user_data)
{
    /* Only one SHT3X instance in the system, assert if more than one is created by the driver */
    static bool allocated = false;
    EAS_ASSERT(!allocated);

    static struct SHT3XStruct inst;
    allocated = true;
    return &inst;
}
