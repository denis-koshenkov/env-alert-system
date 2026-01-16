#include "eas_current_time.h"

static EasTime current_time = 0;

EasTime eas_current_time_get()
{
    return current_time;
}

void fake_eas_current_time_set(EasTime time)
{
    current_time = time;
}
