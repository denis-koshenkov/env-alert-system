#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

/**
 * @brief Monitors whether the system is ready to evaluate alert conditions.
 *
 * Alert conditions should be evaluated only after at least one sample for each variable has been received by the
 * application from the hardware.
 *
 * When at least one sample has been received for each variable, the alert evaluation readiness switches from false to
 * true. When that happens, a callback gets executed, if it was set.
 */

/**
 * @brief Defines callback type to execute when alert evaluation readiness switches from false to true.
 */
typedef void (*AlertEvaluationReadinessReadyCb)();

/**
 * @brief Check whether the system is ready to evaluate alert conditions.
 *
 * @return true The system is ready to evaluate alert conditions.
 * @return false The system is not ready to evaluate alert conditions.
 */
bool alert_evaluation_readiness_is_ready();

/**
 * @brief Notify this module that a temperature sample has been received from the hardware.
 *
 * This function must be called when the first ever sample is received from the hardware. It can also be called for
 * every subsequent sample, but that is not mandatory.
 */
void alert_evaluation_readiness_notify_received_temperature_sample();

/**
 * @brief Notify this module that a pressure sample has been received from the hardware.
 *
 * This function must be called when the first ever sample is received from the hardware. It can also be called for
 * every subsequent sample, but that is not mandatory.
 */
void alert_evaluation_readiness_notify_received_pressure_sample();

/**
 * @brief Notify this module that a humidity sample has been received from the hardware.
 *
 * This function must be called when the first ever sample is received from the hardware. It can also be called for
 * every subsequent sample, but that is not mandatory.
 */
void alert_evaluation_readiness_notify_received_humidity_sample();

/**
 * @brief Notify this module that a light intensity sample has been received from the hardware.
 *
 * This function must be called when the first ever sample is received from the hardware. It can also be called for
 * every subsequent sample, but that is not mandatory.
 */
void alert_evaluation_readiness_notify_received_light_intensity_sample();

/**
 * @brief Set a callback to execute when the system becomes ready to evaluate alert conditions.
 *
 * @param cb Callback to set.
 */
void alert_evaluation_readiness_set_ready_cb(AlertEvaluationReadinessReadyCb cb);

/**
 * @brief Reset the state of this module to the initial state.
 *
 * After a call to this function, this module is in a state where no samples have been received. If a ready callback has
 * been set, it gets cleared.
 */
void alert_evaluation_readiness_reset();

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_EVALUATION_READINESS_H */
