#include <stdbool.h>
#include <stdio.h>

#include "variable_requirement_private.h"
#include "temperature_requirement.h"
#include "variable_requirement_allocator.h"

#include "eas_assert.h"
#include "config.h"
#include "utils/util.h"
#include "hal/temperature.h"
#include "current_temperature.h"

typedef struct TemperatureRequirementStruct *TemperatureRequirement;

struct TemperatureRequirementStruct {
    VariableRequirementStruct base;
    Temperature value;
};

EAS_STATIC_ASSERT(sizeof(struct TemperatureRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

static bool evaluate(VariableRequirement base)
{
    TemperatureRequirement self = (TemperatureRequirement)base;
    Temperature current_temperature = current_temperature_get();

    bool result;
    switch (self->base.operator) {
    case VARIABLE_REQUIREMENT_OPERATOR_GEQ:
        result = (current_temperature >= self->value);
        break;
    case VARIABLE_REQUIREMENT_OPERATOR_LEQ:
        result = (current_temperature <= self->value);
        break;
    default:
        /* Invalid operator */
        EAS_ASSERT(false);
        return false;
    }

    return result;
}

static void destroy(VariableRequirement temperature_requirement)
{
    variable_requirement_allocator_free(temperature_requirement);
}

VariableRequirement temperature_requirement_create(uint8_t alert_id, uint8_t operator, Temperature value)
{
    TemperatureRequirement self = variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self, &interface, operator, alert_id);

    self->value = value;
    return (VariableRequirement)self;
}
