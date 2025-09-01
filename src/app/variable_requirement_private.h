#ifndef ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_PRIVATE_H
#define ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "variable_requirement.h"

typedef struct VariableRequirementInterfaceStruct {
    bool (*evaluate)(VariableRequirement);
    void (*destroy)(VariableRequirement);
} VariableRequirementInterfaceStruct;

typedef struct VariableRequirementStruct {
    VariableRequirementInterfaceStruct *vtable;
    uint8_t operator; /**! Uses values from @ref VariableRequirementOperator, defined as uint8_t to save memory. */
    uint8_t alert_id;
    bool evaluate_has_been_called;
    bool is_result_changed;
    bool previous_evaluation_result;
} VariableRequirementStruct;

/**
 * @brief Create generic variable requirement.
 *
 * This function must be called as early as possible in the create() functions of the subclasses of
 * VariableRequirement, such as TemperatureVariableRequirement. The implementation of this function initializes all
 * members of VariableRequirementStruct. The create() implementation of subclass is only responsible for initializing
 * its own data - it can assume that all members of VariableRequirementStruct are initialized after a call to this
 * function.
 *
 * @note This function must only be called from create() functions of subclasses of variable requirement. It should
 * never be called from anywhere else.
 *
 * @param self Variable requirement instance. This should already point to valid memory, memory allocation for variable
 * requirement instances is performed in the create() functions of the subclasses.
 * @param vtable Interface implementation - initialized and provided by the subclass.
 * @param operator Use one of the values from @ref VariableRequirementOperator. Variable requirement operator to use
 * when evaluating the requirement.
 * @param alert_id Alert id - identifies alert that this variable requirement is a part of.
 */
void variable_requirement_create(VariableRequirement self, VariableRequirementInterfaceStruct *vtable, uint8_t operator,
                                 uint8_t alert_id);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_VARIABLE_REQUIREMENT_PRIVATE_H */
