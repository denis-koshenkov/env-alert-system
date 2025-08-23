#include <stdbool.h>
#include <stdio.h>

#include "variable_requirement_private.h"
#include "humidity_requirement.h"
#include "variable_requirement_allocator.h"

#include "eas_assert.h"
#include "config.h"
#include "utils/util.h"
#include "hal/humidity.h"
#include "current_humidity.h"

typedef struct HumidityRequirementStruct *HumidityRequirement;

struct HumidityRequirementStruct {
    VariableRequirementStruct base;
    Humidity value;
};

EAS_STATIC_ASSERT(sizeof(struct HumidityRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

/**
 * @brief Evaluate humidity variable requirement.
 *
 * @param base Humidity requirement instance returned by @ref humidity_requirement_create.
 *
 * @return true Humidity requirement is currently satisfied.
 * @return false Humidity requirement is currently not satisfied.
 */
static bool evaluate(VariableRequirement base)
{
    HumidityRequirement self = (HumidityRequirement)base;
    Humidity current_humidity = current_humidity_get();

    bool result;
    switch (self->base.operator) {
    case VARIABLE_REQUIREMENT_OPERATOR_GEQ:
        result = (current_humidity >= self->value);
        break;
    case VARIABLE_REQUIREMENT_OPERATOR_LEQ:
        result = (current_humidity <= self->value);
        break;
    default:
        /* Invalid operator */
        EAS_ASSERT(false);
        return false;
    }

    return result;
}

/**
 * @brief Destroy a humidity variable requirement instance.
 *
 * @param base Humidity requirement instance returned by @ref humidity_requirement_create.
 */
static void destroy(VariableRequirement base)
{
    variable_requirement_allocator_free(base);
}

VariableRequirement humidity_requirement_create(uint8_t alert_id, uint8_t operator, Humidity value)
{
    HumidityRequirement self = variable_requirement_allocator_alloc();
    variable_requirement_create((VariableRequirement)self, &interface, operator, alert_id);

    self->value = value;
    return (VariableRequirement)self;
}
