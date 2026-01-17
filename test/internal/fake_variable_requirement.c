#include "variable_requirement_private.h"
#include "fake_variable_requirement.h"
#include "fake_variable_requirement_allocator.h"

#include "config.h"
#include "util.h"

typedef struct FakeVariableRequirementStruct *FakeVariableRequirement;

struct FakeVariableRequirementStruct {
    VariableRequirementStruct base;
    bool evaluate_result;
};

EAS_STATIC_ASSERT(sizeof(struct FakeVariableRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

static bool evaluate(VariableRequirement base)
{
    FakeVariableRequirement self = (FakeVariableRequirement)base;
    return self->evaluate_result;
}

static void destroy(VariableRequirement base)
{
    fake_variable_requirement_allocator_free(base);
}

VariableRequirement fake_variable_requirement_create()
{
    FakeVariableRequirement self = (FakeVariableRequirement)fake_variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self, &interface, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);

    self->evaluate_result = false;
    return (VariableRequirement)self;
}

void fake_variable_requirement_set_evaluate_result(VariableRequirement fake_variable_requirement, bool result)
{
    FakeVariableRequirement self = (FakeVariableRequirement)fake_variable_requirement;
    self->evaluate_result = result;
}
