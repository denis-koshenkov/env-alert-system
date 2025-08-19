#include <stddef.h>

#include "variable_requirement_private.h"
#include "vtable_null_requirement.h"
#include "variable_requirement_allocator.h"

#include "config.h"
#include "utils/util.h"

typedef struct VtableNullRequirementStruct *VtableNullRequirement;

struct VtableNullRequirementStruct {
    VariableRequirementStruct base;
};

EAS_STATIC_ASSERT(sizeof(struct VtableNullRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

VariableRequirement vtable_null_requirement_create()
{
    VtableNullRequirement self = (VtableNullRequirement)variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self, NULL, VARIABLE_REQUIREMENT_OPERATOR_GEQ, 0);

    return (VariableRequirement)self;
}

void vtable_null_requirement_destroy(VariableRequirement vtable_null_requirement)
{
    variable_requirement_allocator_free(vtable_null_requirement);
}
