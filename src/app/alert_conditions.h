#ifndef ALERT_CONDITIONS_H
#define ALERT_CONDITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "alert_condition_defs.h"

/**
 * @brief Stores alert condition instances for each alert.
 *
 * This module creates and stores CONFIG_MAX_NUM_ALERTS alert condition instances. An instance for an alert with a
 * certain alert id can be retrieved by calling @ref alert_conditions_get_alert_condition.
 *
 * # Usage
 * ```
 * // Create instances - should be called just once
 * alert_conditions_create_instances();
 *
 * // Now that the instances are created, they can be retrieved
 * AlertCondition alert_condition_0 = alert_conditions_get_alert_condition(0);
 * AlertCondition alert_condition_1 = alert_conditions_get_alert_condition(1);
 * ```
 */

/**
 * @brief Creates and stores CONFIG_MAX_NUM_ALERTS alert condition instances.
 *
 * Should be called once before @ref alert_conditions_get_alert_condition is called.
 *
 * @note Fires an assert if called more than once. Instances should only be created once.
 */
void alert_conditions_create_instances();

/**
 * @brief Get alert condition instance for an alert.
 *
 * @param alert_id Alert id of the alert.
 *
 * @return AlertCondition Alert condition instance for the alert with id @p alert_id.
 *
 * @note Fires an assert if alert_id >= CONFIG_MAX_NUM_ALERTS is passed.
 * @note Fires an assert if called before @ref alert_conditions_create_instances.
 */
AlertCondition alert_conditions_get_alert_condition(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
