#include "CppUTestExt/MockSupport.h"
#include "eas_time.h"

static bool is_after_or_equal_return_value = false;

EasTime eas_time_get()
{
    return 0;
}

EasTime eas_time_offset_into_future(EasTime time, size_t offset_ms)
{
    return 0;
}

bool eas_time_is_equal_or_after(EasTime time1, EasTime time2)
{
    return is_after_or_equal_return_value;
}

void fake_eas_time_set_is_equal_or_after_return_value(bool return_value)
{
    is_after_or_equal_return_value = return_value;
}
