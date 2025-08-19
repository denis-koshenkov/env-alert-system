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

VariableRequirement mock_variable_requirement_create()
{
    MockVariableRequirement self = (MockVariableRequirement)variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self, &interface, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);

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
