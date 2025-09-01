#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct AlertRaiserStruct *AlertRaiser;

AlertRaiser alert_raiser_create();

void alert_raiser_set_alert(AlertRaiser self, uint8_t alert_id, uint32_t warmup_period_ms, uint32_t cooldown_period_ms);

void alert_raiser_set_alert_condition_result(AlertRaiser self, bool alert_condition_result);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISER_H */
