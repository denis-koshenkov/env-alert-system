#include <stdbool.h>

#include "variable_requirement_private.h"

bool variable_requirement_evaluate(VariableRequirement variable_requirement)
{
    // TODO: test-drive asserts to put here
    return variable_requirement->vtable->evaluate(variable_requirement);
}
