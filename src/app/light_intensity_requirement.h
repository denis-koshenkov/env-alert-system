#ifndef LIGHT_INTENSITY_REQUIREMENT_H
#define LIGHT_INTENSITY_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal/light_intensity.h"
#include "variable_requirement.h"

/**
 * @brief Represents a light intensity variable requirement.
 *
 * Subclass of VariableRequirement.
 */

/**
 * @brief Create a light intensity variable requirement instance.
 *
 * @p operator and @p value together determine when the result of the requirement evaluation. If operator is
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ, then the variable requirement evaluates to true iff the current light intensity
 * value is greater than or equal to @p value. Conversely, if the operator is VARIABLE_REQUIREMENT_OPERATOR_LEQ, the
 * variable requirement evaluates to true iff the current light intensity is less than or equal to @p value.
 *
 * @param alert_id Alert id of the alert to which this light intensity requirement belongs.
 * @param operator Use one of the values from @ref VariableRequirementOperator. Variable requirement operator to use
 * when evaluating the requirement.
 * @param value Light intensity requirement value.
 *
 * @return VariableRequirement Created instance of light intensity requirement.
 */
VariableRequirement light_intensity_requirement_create(uint8_t alert_id, uint8_t operator, LightIntensity value);

#ifdef __cplusplus
}
#endif

#endif
