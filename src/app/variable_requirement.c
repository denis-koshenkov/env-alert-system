#include <stdbool.h>

#include "variable_requirement_private.h"
#include "eas_assert.h"

/**
 * @brief Check that variable requirement operator is valid.
 *
 * @param operator Variable requirement operator.
 *
 * @return true Operator is valid.
 * @return false Operator is invalid.
 */
static bool is_valid_operator(uint8_t operator)
{
    return (operator < VARIABLE_REQUIREMENT_OPERATOR_INVALID);
}

void variable_requirement_create(VariableRequirement self, VariableRequirementInterfaceStruct *vtable, uint8_t operator,
                                 uint8_t alert_id)
{
    EAS_ASSERT(self);
    EAS_ASSERT(vtable);
    EAS_ASSERT(vtable->evaluate);
    EAS_ASSERT(vtable->destroy);
    EAS_ASSERT(is_valid_operator(operator));

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
        /* This is the first ever call to evaluate(). After evaluate() is called once, is_result_changed always returns
         * true, regardless of the result. Before evaluate() was called, there was no result, and after it is called,
         * there IS a result, so the result has changed. */
        self->is_result_changed = true;
    }

    self->previous_evaluation_result = evaluation_result;
    self->evaluate_has_been_called = true;
    return evaluation_result;
}

bool variable_requirement_is_result_changed(VariableRequirement self)
{
    EAS_ASSERT(self);
    /* This function is only allowed to be called if evaluate() has been called at least once. If evaluate() has not
     * been called yet, there is no result yet, so it does not make sense to report whether it has changed. */
    EAS_ASSERT(self->evaluate_has_been_called);
    return self->is_result_changed;
}

uint8_t variable_requirement_get_alert_id(VariableRequirement self)
{
    EAS_ASSERT(self);
    return self->alert_id;
}

void variable_requirement_destroy(VariableRequirement self)
{
    EAS_ASSERT(self);
    EAS_ASSERT(self->vtable);
    EAS_ASSERT(self->vtable->destroy);
    self->vtable->destroy(self);
}
