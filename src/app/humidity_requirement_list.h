#ifndef HUMIDITY_REQUIREMENT_LIST_H
#define HUMIDITY_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "variable_requirement_list_defs.h"

/**
 * @brief Stores humidity requirements of all currently registered alerts in one list.
 */

/**
 * @brief Add a humidity requirement to the list.
 *
 * @param humidity_requirement Humidity requirement to add to the list.
 */
void humidity_requirement_list_add(VariableRequirement humidity_requirement);

/**
 * @brief Execute a callback for each humidity requirement in the list.
 *
 * @param cb Callback to execute for each humidity requirement currently in the list.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void humidity_requirement_list_for_each(VariableRequirementListForEachCb cb);

/**
 * @brief Remove all humidity requirements from the list that belong to a specific alert.
 *
 * @param alert_id All humidity requirements that belong to the alert with this id are removed from the list.
 */
void humidity_requirement_list_remove_all_for_alert(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif
