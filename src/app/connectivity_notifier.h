#ifndef ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H
#define ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

void connectivity_notifier_notify(uint8_t alert_id, bool is_raised);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_CONNECTIVITY_NOTIFIER_H */
