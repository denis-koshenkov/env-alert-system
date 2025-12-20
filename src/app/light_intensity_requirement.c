#include <stdbool.h>
#include <stdio.h>

#include "variable_requirement_private.h"
#include "light_intensity_requirement.h"
#include "variable_requirement_allocator.h"

#include "eas_assert.h"
#include "config.h"
#include "util.h"
#include "light_intensity.h"
#include "current_light_intensity.h"

typedef struct LightIntensityRequirementStruct *LightIntensityRequirement;

struct LightIntensityRequirementStruct {
    VariableRequirementStruct base;
    LightIntensity value;
};

EAS_STATIC_ASSERT(sizeof(struct LightIntensityRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);

// Forward declarations of interface functions to define the interface.
static bool evaluate(VariableRequirement base);
static void destroy(VariableRequirement base);

static VariableRequirementInterfaceStruct interface = {
    .evaluate = evaluate,
    .destroy = destroy,
};

/**
 * @brief Evaluate light intensity variable requirement.
 *
 * @param base LightIntensity requirement instance returned by @ref light_intensity_requirement_create.
 *
 * @return true Light intensity requirement is currently satisfied.
 * @return false Light intensity requirement is currently not satisfied.
 */
static bool evaluate(VariableRequirement base)
{
    LightIntensityRequirement self = (LightIntensityRequirement)base;
    LightIntensity current_light_intensity = current_light_intensity_get();

    bool result;
    switch (self->base.operator) {
    case VARIABLE_REQUIREMENT_OPERATOR_GEQ:
        result = (current_light_intensity >= self->value);
        break;
    case VARIABLE_REQUIREMENT_OPERATOR_LEQ:
        result = (current_light_intensity <= self->value);
        break;
    default:
        /* Invalid operator */
        EAS_ASSERT(false);
        return false;
    }

    return result;
}

/**
 * @brief Destroy a light intensity variable requirement instance.
 *
 * @param base LightIntensity requirement instance returned by @ref light_intensity_requirement_create.
 */
static void destroy(VariableRequirement base)
{
    variable_requirement_allocator_free(base);
}

VariableRequirement light_intensity_requirement_create(uint8_t alert_id, uint8_t operator, LightIntensity value)
{
    LightIntensityRequirement self = variable_requirement_allocator_alloc();
    EAS_ASSERT(self);
    variable_requirement_create((VariableRequirement)self, &interface, operator, alert_id);

    self->value = value;
    return (VariableRequirement)self;
}
