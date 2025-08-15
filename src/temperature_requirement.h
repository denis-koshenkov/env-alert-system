#ifndef TEMPERATURE_REQUIREMENT_H
#define TEMPERATURE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

VariableRequirement temperature_requirement_create(uint8_t alert_id, VariableRequirementOperator operator);

#ifdef __cplusplus
}
#endif

#endif
