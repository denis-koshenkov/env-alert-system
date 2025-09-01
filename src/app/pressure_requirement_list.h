#ifndef PRESSURE_REQUIREMENT_LIST_H
#define PRESSURE_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "variable_requirement_list_defs.h"

/**
 * @brief Stores pressure requirements of all currently registered alerts in one list.
 */

/**
 * @brief Add a pressure requirement to the list.
 *
 * @param pressure_requirement Pressure requirement to add to the list.
 */
void pressure_requirement_list_add(VariableRequirement pressure_requirement);

/**
 * @brief Execute a callback for each pressure requirement in the list.
 *
 * @param cb Callback to execute for each pressure requirement currently in the list.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void pressure_requirement_list_for_each(VariableRequirementListForEachCb cb);

/**
 * @brief Remove all pressure requirements from the list that belong to a specific alert.
 *
 * @param alert_id All pressure requirements that belong to the alert with this id are removed from the list.
 */
void pressure_requirement_list_remove_all_for_alert(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
