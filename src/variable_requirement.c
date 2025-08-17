#include <stdbool.h>

#include "variable_requirement_private.h"
#include "eas_assert.h"

bool variable_requirement_evaluate(VariableRequirement variable_requirement)
{
    EAS_ASSERT(variable_requirement);
    EAS_ASSERT(variable_requirement->vtable);
    EAS_ASSERT(variable_requirement->vtable->evaluate);
    return variable_requirement->vtable->evaluate(variable_requirement);
}
