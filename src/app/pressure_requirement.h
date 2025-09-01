#ifndef ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_REQUIREMENT_H
#define ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/pressure.h"
#include "variable_requirement.h"

/**
 * @brief Represents a pressure variable requirement.
 *
 * Subclass of VariableRequirement.
 */

/**
 * @brief Create a pressure variable requirement instance.
 *
 * @p operator and @p value together determine when the result of the requirement evaluation. If operator is
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ, then the variable requirement evaluates to true iff the current pressure value
 * is greater than or equal to @p value. Conversely, if the operator is VARIABLE_REQUIREMENT_OPERATOR_LEQ, the variable
 * requirement evaluates to true iff the current pressure is less than or equal to @p value.
 *
 * @param alert_id Alert id of the alert to which this pressure requirement belongs.
 * @param operator Use one of the values from @ref VariableRequirementOperator. Variable requirement operator to use
 * when evaluating the requirement.
 * @param value Pressure requirement value.
 *
 * @return VariableRequirement Created instance of pressure requirement.
 */
VariableRequirement pressure_requirement_create(uint8_t alert_id, uint8_t operator, Pressure value);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PRESSURE_REQUIREMENT_H */
