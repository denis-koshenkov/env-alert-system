#ifndef ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_EVALUATOR_H
#define ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_EVALUATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "variable_requirement_defs.h"

/**
 * @brief Evaluates a variable requirement.
 *
 * This function is a stable abstract interface to be used by the AlertCondition module.
 *
 * AlertCondition module calls this function, passes the VariableRequirement instance, and gets back the evaluation
 * result of this variable requirement.
 *
 * This way, AlertCondition does not need to know about the public variable requirement API defined in
 * variable_requirement.h. AlertCondition treats all VariableRequirement instances as black boxes. VariableRequirement
 * instances get added to the AlertCondition, AlertCondition stores them, and calls this function to evaluate them. It
 * does not use them in any other way.
 *
 * @param variable_requirement Variable requirement to evaluate.
 *
 * @return true Variable requirement evaluated to true.
 * @return false Variable requirement evaluated to false.
 */
bool variable_requirement_evaluator_evaluate(VariableRequirement variable_requirement);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_EVALUATOR_H */
