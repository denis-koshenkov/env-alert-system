#include <stdbool.h>
#include <stddef.h>

#include "get_inst_buf.h"
#include "bmp280_private.h"

void *bmp280_driver_get_inst_buf(void *user_data)
{
    /* Only one BMP280 instance in the system, return NULL if more than one is created by the driver */
    static bool allocated = false;
    static struct BMP280Struct inst;

    void *ret = NULL;
    if (!allocated) {
        allocated = true;
        ret = &inst;
    }
    return ret;
}
