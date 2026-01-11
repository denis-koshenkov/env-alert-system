#include <stddef.h>

#include "variable_requirement_private.h"
#include "mock_variable_requirement.h"
#include "variable_requirement_allocator.h"

#include "config.h"
#include "util.h"

typedef struct MockVariableRequirementStruct *MockVariableRequirement;

struct MockVariableRequirementStruct {
    VariableRequirementStruct base;
    bool evaluate_result;
};

EAS_STATIC_ASSERT(sizeof(struct MockVariableRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct valid_interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

static VariableRequirementInterfaceStruct interface_with_evaluate_null = {
    .evaluate = NULL,
    .destroy = destroy,
};

static VariableRequirementInterfaceStruct interface_with_destroy_null = {
    .evaluate = evaluate,
    .destroy = NULL,
};

static bool evaluate(VariableRequirement base)
{
    MockVariableRequirement self = (MockVariableRequirement)base;
    return self->evaluate_result;
}

static void destroy(VariableRequirement base)
{
    variable_requirement_allocator_free(base);
}

VariableRequirement mock_variable_requirement_create(bool pass_null_instance_to_var_req_create,
                                                     bool pass_invalid_operator_to_var_req_create,
                                                     uint8_t vtable_parameter)
{
    /* Allocate memory regardless of whether we are passing NULL to self or not, because the test always expects us to
     * allocate - it calls free() in teardown. */
    MockVariableRequirement self = (MockVariableRequirement)variable_requirement_allocator_alloc();
    if (pass_null_instance_to_var_req_create) {
        self = NULL;
    }
    uint8_t operator = pass_invalid_operator_to_var_req_create ? VARIABLE_REQUIREMENT_OPERATOR_INVALID
                                                               : VARIABLE_REQUIREMENT_OPERATOR_GEQ;

    VariableRequirementInterfaceStruct *vtable = NULL;
    switch (vtable_parameter) {
    case MOCK_VARIABLE_REQUIREMENT_PASS_NULL_VTABLE_TO_VAR_REQ_CREATE:
        vtable = NULL;
        break;
    case MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE:
        vtable = &valid_interface;
        break;
    case MOCK_VARIABLE_REQUIREMENT_PASS_NULL_EVALUATE_TO_VAR_REQ_CREATE:
        vtable = &interface_with_evaluate_null;
        break;
    case MOCK_VARIABLE_REQUIREMENT_PASS_NULL_DESTROY_TO_VAR_REQ_CREATE:
        vtable = &interface_with_destroy_null;
        break;
    default:
        break;
    }

    variable_requirement_create((VariableRequirement)self, vtable, operator, 0);

    self->evaluate_result = false;
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
