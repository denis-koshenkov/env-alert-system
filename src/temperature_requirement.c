#include <stdbool.h>
#include <stdio.h>

#include "variable_requirement_private.h"
#include "temperature_requirement.h"
#include "variable_requirement_allocator.h"

#include "config.h"
#include "utils/util.h"
#include "hal/temperature.h"
#include "current_temperature.h"

typedef struct TemperatureRequirementStruct *TemperatureRequirement;

struct TemperatureRequirementStruct {
    VariableRequirementStruct base;
};

EAS_STATIC_ASSERT(sizeof(struct TemperatureRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
};

static bool evaluate(VariableRequirement base)
{
    Temperature current_temperature = current_temperature_get();
    return true;
}

VariableRequirement temperature_requirement_create(uint8_t alert_id, VariableRequirementOperator operator,
                                                   Temperature value)
{
    TemperatureRequirement self = variable_requirement_allocator_alloc();
    self->base.vtable = &interface;
    return (VariableRequirement)self;
}

void temperature_requirement_destroy(VariableRequirement temperature_requirement)
{
    variable_requirement_allocator_free(temperature_requirement);
}
