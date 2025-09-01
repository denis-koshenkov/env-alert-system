#include "variable_requirement_evaluator.h"
#include "eas_assert.h"
#include "variable_requirement.h"

bool variable_requirement_evaluator_evaluate(VariableRequirement variable_requirement)
{
    EAS_ASSERT(variable_requirement);
    return variable_requirement_evaluate(variable_requirement);
}
