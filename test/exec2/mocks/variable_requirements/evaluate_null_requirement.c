#include <stddef.h>

#include "variable_requirement_private.h"
#include "evaluate_null_requirement.h"
#include "variable_requirement_allocator.h"

#include "config.h"
#include "utils/util.h"

typedef struct EvaluateNullRequirementStruct *EvaluateNullRequirement;

struct EvaluateNullRequirementStruct {
    VariableRequirementStruct base;
};

EAS_STATIC_ASSERT(sizeof(struct EvaluateNullRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = NULL,
};

VariableRequirement evaluate_null_requirement_create()
{
    EvaluateNullRequirement self = (EvaluateNullRequirement)variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self);

    self->base.vtable = &interface;
    return (VariableRequirement)self;
}

void evaluate_null_requirement_destroy(VariableRequirement evaluate_null_requirement)
{
    variable_requirement_allocator_free(evaluate_null_requirement);
}
