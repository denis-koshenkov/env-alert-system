#include <stdbool.h>

#include "get_instance_memory.h"
#include "bh1750_private.h"

void *bh1750_driver_get_instance_memory(void *user_data)
{
    /* Only one BH1750 instance in the system, return NULL if more than one is created by the driver */
    static bool allocated = false;
    static struct BH1750Struct inst;

    void *ret = NULL;
    if (!allocated) {
        allocated = true;
        ret = &inst;
    }
    return ret;
}
