#include <zephyr/kernel.h>

#include "eas_current_time.h"

EasTime eas_current_time_get()
{
    /* 2^32 ms is ~49 24-hour days. Our application can run longer than that, so this can overflow. EasTime module takes
     * this into account when doing time calculations, so that is not an issue. Using 32-bit version because EasTime is
     * implemented to operate on 32-bit time variables. */
    return k_uptime_get_32();
}
