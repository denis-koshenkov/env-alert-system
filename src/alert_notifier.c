#include "alert_notifier.h"

void alert_notifier_notify(uint8_t alert_id, bool is_raised)
{
    /* Empty for now, because unit tests mock this interface instead of using the real one (this one). The
     * implementation here should call connectivity_notifier_notify and led_notifier_notify functions so that they
     * execute their respective notifications via connectivity and LED. */
}
