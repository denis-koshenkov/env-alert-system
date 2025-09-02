#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISERS_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISERS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "alert_raiser_defs.h"

/**
 * @brief Stores alert raiser instances for each alert.
 *
 * This module creates and stores CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE alert raiser instances. An instance for
 * an alert with a certain alert id can be retrieved by calling @ref alert_raisers_get_alert_raiser.
 *
 * # Usage
 * ```
 * // Create instances - should be called just once
 * alert_raisers_create_instances();
 *
 * // Now that the instances are created, they can be retrieved
 * AlertRaiser alert_raiser_0 = alert_raisers_get_alert_raiser(0);
 * AlertRaiser alert_raiser_1 = alert_raisers_get_alert_raiser(1);
 * ```
 */

/**
 * @brief Creates and stores CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE alert raiser instances.
 *
 * Should be called once before @ref alert_raisers_get_alert_raiser is called.
 *
 * @note Fires an assert if called more than once. Instances should only be created once.
 */
void alert_raisers_create_instances();

/**
 * @brief Get alert raiser instance for an alert.
 *
 * @param alert_id Alert id of the alert.
 *
 * @return AlertRaiser Alert raiser instance for the alert with id @p alert_id.
 *
 * @note Fires an assert if alert_id >= CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE is passed.
 * @note Fires an assert if called before @ref alert_raisers_create_instances.
 */
AlertRaiser alert_raisers_get_alert_raiser(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_RAISERS_H */
