#include <stdbool.h>

#include "variable_requirement_private.h"
#include "eas_assert.h"

void variable_requirement_create(VariableRequirement self)
{
    self->evaluate_has_been_called = false;
}

bool variable_requirement_evaluate(VariableRequirement self)
{
    EAS_ASSERT(self);
    EAS_ASSERT(self->vtable);
    EAS_ASSERT(self->vtable->evaluate);

    self->evaluate_has_been_called = true;
    return self->vtable->evaluate(self);
}

bool variable_requirement_is_result_changed(VariableRequirement self)
{
    EAS_ASSERT(self->evaluate_has_been_called);
    return true;
}
