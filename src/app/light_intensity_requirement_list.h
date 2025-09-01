#ifndef ENV_ALERT_SYSTEM_SRC_APP_LIGHT_INTENSITY_REQUIREMENT_LIST_H
#define ENV_ALERT_SYSTEM_SRC_APP_LIGHT_INTENSITY_REQUIREMENT_LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "variable_requirement_list_defs.h"

/**
 * @brief Stores light intensity requirements of all currently registered alerts in one list.
 */

/**
 * @brief Add a light intensity requirement to the list.
 *
 * @param light intensity_requirement Light intensity requirement to add to the list.
 */
void light_intensity_requirement_list_add(VariableRequirement light_intensity_requirement);

/**
 * @brief Execute a callback for each light intensity requirement in the list.
 *
 * @param cb Callback to execute for each light intensity requirement currently in the list.
 *
 * @note Fires an assert if @p cb is NULL. Calling this function with @p cb equal to NULL would be equivalent to not
 * calling this function at all.
 */
void light_intensity_requirement_list_for_each(VariableRequirementListForEachCb cb);

/**
 * @brief Remove all light intensity requirements from the list that belong to a specific alert.
 *
 * @param alert_id All light intensity requirements that belong to the alert with this id are removed from the list.
 */
void light_intensity_requirement_list_remove_all_for_alert(uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_LIGHT_INTENSITY_REQUIREMENT_LIST_H */
