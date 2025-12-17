#include <stdbool.h>
#include <stdio.h>

#include "variable_requirement_private.h"
#include "pressure_requirement.h"
#include "variable_requirement_allocator.h"

#include "eas_assert.h"
#include "config.h"
#include "utils/util.h"
#include "pressure.h"
#include "current_pressure.h"

typedef struct PressureRequirementStruct *PressureRequirement;

struct PressureRequirementStruct {
    VariableRequirementStruct base;
    Pressure value;
};

EAS_STATIC_ASSERT(sizeof(struct PressureRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

/**
 * @brief Evaluate pressure variable requirement.
 *
 * @param base Pressure requirement instance returned by @ref pressure_requirement_create.
 *
 * @return true Pressure requirement is currently satisfied.
 * @return false Pressure requirement is currently not satisfied.
 */
static bool evaluate(VariableRequirement base)
{
    PressureRequirement self = (PressureRequirement)base;
    Pressure current_pressure = current_pressure_get();

    bool result;
    switch (self->base.operator) {
    case VARIABLE_REQUIREMENT_OPERATOR_GEQ:
        result = (current_pressure >= self->value);
        break;
    case VARIABLE_REQUIREMENT_OPERATOR_LEQ:
        result = (current_pressure <= self->value);
        break;
    default:
        /* Invalid operator */
        EAS_ASSERT(false);
        return false;
    }

    return result;
}

/**
 * @brief Destroy a pressure variable requirement instance.
 *
 * @param base Pressure requirement instance returned by @ref pressure_requirement_create.
 */
static void destroy(VariableRequirement base)
{
    variable_requirement_allocator_free(base);
}

VariableRequirement pressure_requirement_create(uint8_t alert_id, uint8_t operator, Pressure value)
{
    PressureRequirement self = variable_requirement_allocator_alloc();
    EAS_ASSERT(self);
    variable_requirement_create((VariableRequirement)self, &interface, operator, alert_id);

    self->value = value;
    return (VariableRequirement)self;
}
