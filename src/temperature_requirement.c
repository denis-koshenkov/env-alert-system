#include <stdbool.h>

#include "variable_requirement_private.h"
#include "temperature_requirement.h"
#include "hal/temperature.h"

typedef struct TemperatureRequirementStruct *TemperatureRequirement;

struct TemperatureRequirementStruct {
    VariableRequirementStruct base;
    /**< Together with the operator defined in the base struct, this value defines the requirement: e.g. ">= 20", where
     * >= is the operator, and 20 is the value. */
    temperature temperature_value;
};

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
};

static bool evaluate(VariableRequirement base)
{
    TemperatureRequirement self = (TemperatureRequirement)base;
    // TODO: implement evaluating temperature requirement
    return false;
}

// TODO: Move to its own allocator module.
static TemperatureRequirement temperature_requirement_allocator_allocate()
{
}

VariableRequirement temperature_requirement_create(uint8_t alert_id, VariableRequirementOperator operator)
{
    TemperatureRequirement self = temperature_requirement_allocator_allocate();
    self->base.vtable = &interface;
    self->base.alert_id = alert_id;
    self->base.operator= operator;
    return (VariableRequirement)self;
}
