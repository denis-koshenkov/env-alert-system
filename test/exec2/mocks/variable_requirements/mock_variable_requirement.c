#include <stddef.h>

#include "variable_requirement_private.h"
#include "mock_variable_requirement.h"
#include "variable_requirement_allocator.h"

#include "config.h"
#include "utils/util.h"

typedef struct MockVariableRequirementStruct *MockVariableRequirement;

struct MockVariableRequirementStruct {
    VariableRequirementStruct base;
    bool evaluate_result;
};

EAS_STATIC_ASSERT(sizeof(struct MockVariableRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
};

static bool evaluate(VariableRequirement base)
{
    MockVariableRequirement self = (MockVariableRequirement)base;
    return self->evaluate_result;
}

VariableRequirement mock_variable_requirement_create(bool pass_null_instance_to_var_req_create,
                                                     bool pass_invalid_operator_to_var_req_create,
                                                     bool pass_null_vtable)
{
    /* Allocate memory regardless of whether we are passing NULL to self or not, because the test always expects us to
     * allocate - it calls free() in teardown. */
    MockVariableRequirement self = (MockVariableRequirement)variable_requirement_allocator_alloc();
    if (pass_null_instance_to_var_req_create) {
        self = NULL;
    }
    VariableRequirementOperator operator = pass_invalid_operator_to_var_req_create
                                               ? VARIABLE_REQUIREMENT_OPERATOR_INVALID
                                               : VARIABLE_REQUIREMENT_OPERATOR_GEQ;
    VariableRequirementInterfaceStruct *vtable = pass_null_vtable ? NULL : &interface;

    variable_requirement_create((VariableRequirement)self, vtable, operator, 0);
    return (VariableRequirement)self;
}

void mock_variable_requirement_destroy(VariableRequirement mock_variable_requirement)
{
    variable_requirement_allocator_free(mock_variable_requirement);
}

void mock_variable_requirement_set_evaluate_result(VariableRequirement mock_variable_requirement, bool result)
{
    MockVariableRequirement self = (MockVariableRequirement)mock_variable_requirement;
    self->evaluate_result = result;
}
