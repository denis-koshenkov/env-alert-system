#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "alert_condition_defs.h"
#include "variable_requirement_defs.h"

/**
 * @brief Alert condition module.
 *
 * The evaluation result of the alert condition is one of the factors that determines whether an alert is considered
 * raised or silenced.
 *
 * An alert condition is a list of variable requirements. These variable requirements can have AND and OR logical
 * operators between them. The evaluation result that is obtained by calling @ref alert_condition_evaluate is the result
 * of evaluating the logical expression that is formed by the variable requirements and the AND and OR logical
 * operators between them.
 *
 * An alert condition is a list of ORed variable requirements separate by AND logical operators. For example:
 * ```
 *   (req_1 OR req_2) AND (req_3 OR req_4)
 * ```
 *
 * A typical usage flow for this module:
 *   -# An alert condition instance is created
 *   -# Variable requirements are added, with signals in between the different ORed conditions. This is done by calling
 * @ref alert_condition_start_new_ored_requirement, as we will see in the below example.
 *   -# The alert condition is evaluated.
 *
 * Here is a code snippet that showcases this usage flow using the example alert condition above:
 * ```
 * // Create alert condition
 * AlertCondition alert_condition = alert_condition_create();
 *
 * // Add all variable requirements
 * alert_condition_add_variable_requirement(alert_condition, req_1);
 * alert_condition_add_variable_requirement(alert_condition, req_2);
 * // Signal the end of the first ORed requirement and the beginning of the next one
 * alert_condition_start_new_ored_requirement(alert_condition);
 * alert_condition_add_variable_requirement(alert_condition, req_3);
 * alert_condition_add_variable_requirement(alert_condition, req_4);
 *
 * // Evaluate the condition
 * bool evaluation_result = alert_condition_evaluate(alert_condition);
 * ```
 *
 * For every AND in the logical expression, @ref alert_condition_start_new_ored_requirement needs to be called. It is
 * also allowed to call @ref alert_condition_start_new_ored_requirement before any variable requirements are added and
 * after all variable requirements have been added to the condition - such calls have no effect on the evaluation
 * result.
 *
 * Note that it is not possible to directly add a condition that has OR operators that take precedence over AND, for
 * example:
 * ```
 *   (req_1 AND req_2) OR (req_3 AND req_4)
 * ```
 *
 * The good news is is that such logical expressions can always be converted to equivalent expressions that have AND
 * operators that take precedence over OR operators. Those expressions can then be directly represetned by this module.
 * Here is an example conversion to a logical expression that can be represented by this module directly:
 * ```
 *   ((req_1 AND req_2) OR (req_3 AND req_4)) <=> ((req_1 AND req_2) OR req_3) AND ((req_1 AND req_2) OR req_4) <=>
 *   (req_1 OR req_3) AND (req_2 OR req_3) AND (req_1 OR req_4) AND (req_2 or req_4)
 * ```
 *
 * @ref alert_condition_for_each can be used when a certain action needs to be performed for every variable requirement
 * in the alert condition.
 *
 * @ref alert_condition_reset can be used to completely reset the alert condition instance to the state as if the
 * instance had just been created. All variable requirements are removed as a result of calling @ref
 * alert_condition_reset.
 */

/**
 * @brief Defines callback type to execute for each variable requirement as a part of @ref alert_condition_for_each.
 *
 * @ref alert_condition_for_each executes this callback for each variable requirement present in the alert condition.
 *
 * @param variable_requirement Variable requirement instance for which the callback is being executed.
 */
typedef void (*AlertConditionForEachCb)(VariableRequirement variable_requirement);

/**
 * @brief Create an alert condition instance.
 *
 * @return AlertCondition Created alert condition instance.
 */
AlertCondition alert_condition_create();

/**
 * @brief Add a variable requirement to the alert condition.
 *
 * Raises an assert if CONFIG_ALERT_CONDITION_MAX_NUM_VARIABLE_REQUIREMENTS variable requirements are already added to
 * the alert condition.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 * @param variable_requirement Variable requirement to add to the condition.
 */
void alert_condition_add_variable_requirement(AlertCondition self, VariableRequirement variable_requirement);

/**
 * @brief Singal that the next variable requirement added to the condition should belong to a new ORed requirement.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 */
void alert_condition_start_new_ored_requirement(AlertCondition self);

/**
 * @brief Evaluate the alert condition.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 *
 * @return true Alert condition evaluted to true.
 * @return false Alert condition evaluted to false.
 *
 * @note Fires an assert if the condition contains no variable requirements.
 */
bool alert_condition_evaluate(AlertCondition self);

/**
 * @brief Execute a callback for each variable requirement currently in the alert condition.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 * @param cb Callback to execute for each variable requirement.
 */
void alert_condition_for_each(AlertCondition self, AlertConditionForEachCb cb);

/**
 * @brief Restore the alert condition as if the instance was just created.
 *
 * Removes all variable requirements from the alert condition.
 *
 * @param self Alert condition instance returned by @ref alert_condition_create.
 */
void alert_condition_reset(AlertCondition self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_H */
