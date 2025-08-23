#ifndef HUMIDITY_REQUIREMENT_H
#define HUMIDITY_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/humidity.h"
#include "variable_requirement.h"

/**
 * @brief Represents a humidity variable requirement.
 *
 * Subclass of VariableRequirement.
 */

/**
 * @brief Create a humidity variable requirement instance.
 *
 * @p operator and @p value together determine when the result of the requirement evaluation. If operator is
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ, then the variable requirement evaluates to true iff the current humidity value
 * is greater than or equal to @p value. Conversely, if the operator is VARIABLE_REQUIREMENT_OPERATOR_LEQ, the variable
 * requirement evaluates to true iff the current humidity is less than or equal to @p value.
 *
 * @param alert_id Alert id of the alert to which this humidity requirement belongs.
 * @param operator Use one of the values from @ref VariableRequirementOperator. Variable requirement operator to use
 * when evaluating the requirement.
 * @param value Humidity requirement value.
 *
 * @return VariableRequirement Created instance of humidity requirement.
 */
VariableRequirement humidity_requirement_create(uint8_t alert_id, uint8_t operator, Humidity value);

#ifdef __cplusplus
}
#endif

#endif
