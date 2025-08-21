#ifndef TEMPERATURE_REQUIREMENT_H
#define TEMPERATURE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/temperature.h"
#include "variable_requirement.h"

/**
 * @brief Represents a temperature variable requirement.
 *
 * Subclass of VariableRequirement.
 */

/**
 * @brief Create a temperature variable requirement instance.
 *
 * @p operator and @p value together determine when the result of the requirement evaluation. If operator is
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ, then the variable requirement evaluates to true iff the current temperature value
 * is greater than or equal to @p value. Conversely, if the operator is VARIABLE_REQUIREMENT_OPERATOR_LEQ, the variable
 * requirement evaluates to true iff the current temperature is less than or equal to @p value.
 *
 * @param alert_id Alert id of the alert to which this temperature requirement belongs.
 * @param operator Use one of the values from @ref VariableRequirementOperator. Variable requirement operator to use
 * when evaluating the requirement.
 * @param value Temperature requirement value.
 *
 * @return VariableRequirement
 */
VariableRequirement temperature_requirement_create(uint8_t alert_id, uint8_t operator, Temperature value);

#ifdef __cplusplus
}
#endif

#endif
