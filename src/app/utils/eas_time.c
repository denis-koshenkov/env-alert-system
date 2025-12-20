#include "utils/eas_time.h"
#include "util.h"

/* The implementation assumes that EasTime is an unsigned type of 4 bytes, and the max value that can be represented by
 * that type is 0xFFFFFFFF. */
EAS_STATIC_ASSERT(sizeof(EasTime) == sizeof(uint32_t));

EasTime eas_time_offset_into_future(EasTime time, size_t offset_ms)
{
    return time + offset_ms;
}

bool eas_time_is_equal_or_after(EasTime time1, EasTime time2)
{
    EasTime diff = time1 - time2;
    return (diff <= (UINT32_MAX / 2));
}
