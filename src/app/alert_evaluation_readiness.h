#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

bool alert_evaluation_readiness_is_ready();

void alert_evaluation_readiness_notify_received_temperature_sample();

void alert_evaluation_readiness_notify_received_pressure_sample();

void alert_evaluation_readiness_notify_received_humidity_sample();

void alert_evaluation_readiness_notify_received_light_intensity_sample();

void alert_evaluation_readiness_reset();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H */
