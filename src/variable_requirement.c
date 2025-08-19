#include <stdbool.h>

#include "variable_requirement_private.h"
#include "eas_assert.h"

void variable_requirement_create(VariableRequirement self, VariableRequirementInterfaceStruct *vtable,
                                 VariableRequirementOperator operator, uint8_t alert_id)
{
    self->vtable = vtable;
    self->operator = operator;
    self->alert_id = alert_id;

    self->evaluate_has_been_called = false;
    self->is_result_changed = true;
    self->previous_evaluation_result = false;
}

bool variable_requirement_evaluate(VariableRequirement self)
{
    EAS_ASSERT(self);
    EAS_ASSERT(self->vtable);
    EAS_ASSERT(self->vtable->evaluate);

    bool evaluation_result = self->vtable->evaluate(self);

    if (self->evaluate_has_been_called) {
        self->is_result_changed = (evaluation_result != self->previous_evaluation_result);
    } else {
        self->is_result_changed = true;
    }

    self->previous_evaluation_result = evaluation_result;
    self->evaluate_has_been_called = true;
    return evaluation_result;
}

bool variable_requirement_is_result_changed(VariableRequirement self)
{
    EAS_ASSERT(self);
    EAS_ASSERT(self->evaluate_has_been_called);
    return self->is_result_changed;
}

uint8_t variable_requirement_get_alert_id(VariableRequirement self)
{
    EAS_ASSERT(self);
    return self->alert_id;
}
