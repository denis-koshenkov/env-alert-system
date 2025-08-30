#ifndef VARIABLE_REQUIREMENT_H
#define VARIABLE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "variable_requirement_defs.h"

/// Variable requirement operator.
typedef enum VariableRequirementOperator {
    /// Greater or equal to.
    VARIABLE_REQUIREMENT_OPERATOR_GEQ,
    /// Less than or equal to.
    VARIABLE_REQUIREMENT_OPERATOR_LEQ,

    /// Invalid operator - do not use.
    VARIABLE_REQUIREMENT_OPERATOR_INVALID,
} VariableRequirementOperator;

/**
 * @brief Abstract class that represents a variable requirement.
 *
 * # Introduction
 *
 * An example variable requirement is "temperature >= 25". This requirement would evaluate to true if current
 * temperature value >= 25. If the current temperature value is < 25, this requirement would evaluate to false.
 *
 * Subclasses of this class represent variable requirements of specific variables - e.g. temperature, pressure, etc.
 * This class represents a generic variable requirement - it does not know which specific variable the requirement is
 * about. Subclasses implement their own evaluate() function - that function is called in the implementation of @ref
 * variable_requirement_evaluate.
 *
 * The main reason why subclasses are necessary is that specific variable requirements need to store the requirement
 * value. The temperature requirement example above would have the requirement value 25. However, the sizes of data
 * types for different variables are different, so they cannot be generically stored in one class. Because of this, the
 * requirement values are stored as private members in the subclasses.
 *
 * This class is responsible for logic that is common for all variable requirements, regardless of which variable they
 * represent. For example, report whether the evaluation result from the last evaluation is different from the second to
 * last evalution. Another example is retrieving the alert id of the alert to which this variable requirement belongs.
 *
 * # Usage
 *
 * The following example shows how to instantiate and use a variable requirement. The example is based on the
 * TemperatureVariableRequirement subclass, but it applies to any subclass of VariableRequirement.
 * ```
 * // Create temperature variable requirement. Uses the create() of the subclass function. variable_requirement_create()
 * // should never be called directly - hence it is not present in this file as a public function.
 * VariableRequirement temperature_requirement = temperature_requirement_create(alert_id, operator, requirement_value);
 *
 * // After creating, the VariableRequirement public functions can be used. Evaluate:
 * bool evaluate_result_1 = variable_requirement_evaluate(temperature_requirement);
 *
 * // is_result_changed_1 will always return true, regardless of the evaluation result, because it is called
 * // after variable_requirement_evaluate has been called only once.
 * bool is_result_changed_1 = variable_requirement_is_result_changed;
 *
 * // Evaluate again
 * bool evaluate_result_2 = variable_requirement_evaluate(temperature_requirement);
 *
 * // Check whether evaluate_result_1 is different from evaluate_result_2
 * bool is_result_changed = variable_requirement_is_result_changed(temperature_requirement);
 *
 * // Get alert id
 * uint8_t alert_id = variable_requirement_get_alert_id(temperature_requirement);
 *
 * // Destroy variable requirement
 * variable_requirement_destroy(temperature_requirement);
 * ```
 *
 * # How to create a subclass
 *
 * 1. Define a evaluate() function, a destroy() function, and define an interface (vtable) that contains a pointer to
 * these functions. This vtable will be passed as an argument to the variable_requirement_create() call which happens
 * inside the create() function of the subclass.
 * ```
 * static bool evaluate(VariableRequirement base);
 * static VariableRequirementInterfaceStruct interface = {
 *     .evaluate = evaluate,
 *     .destroy = destroy,
 * };
 *
 * static bool evaluate(VariableRequirement base)
 * {
 *     // evaluate() implementation of the subclass
 * }
 *
 * static void destroy(VariableRequirement base)
 * {
 *     // destroy() implementation of the subclass
 * }
 * ```
 *
 * The destroy() function should free the memory that is allocated during a call to the create() function of the
 * subclass. See step 4.1 below for more details.
 *
 * 2. Define the subclass struct. The purpose of this struct is to hold all of the private members defined in
 * VariableRequirementStruct, as well as the requirement value. Example struct definition:
 * ```
 * typedef struct MyRequirementStruct *MyRequirement;
 * struct MyRequirementStruct {
 *     VariableRequirementStruct base;
 *     MyValueType value;
 * };
 * ```
 *
 * 3. Use static assert to ensure that the size of MyRequirementStruct does not exceed the size defined in the config:
 * ```
 * EAS_STATIC_ASSERT(sizeof(struct MyRequirementStruct) <= CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);
 * ```
 * If the build fails because of this assert, increase CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE until the build succeeds.
 *
 * 4. Define the subclass create() function. Inside this create function, the following needs to happen:
 *   1. Allocate memory that will fit MyRequirementStruct.
 *   2. Call variable_requirement_create() and pass the allocated memory as a parameter. Also pass the vtable, alert id,
 * and operator. vtable is the interface we defined in step 1. Alert id and operator are probably passed as arguments to
 * the create() functions of the subclass, but that is not mandatory.
 *   3. Do anything else that needs to be done to initialize the private members in MyRequirementStruct (except for
 * base, because all of the data it contains is initialized when variable_requirement_create() is called in step 2). For
 * example, set the requirement value, as shown in the example below.
 *   4. Return the memory allocated in step 1 as VariableRequirement type, which is simply a pointer to
 * VariableRequirementStruct. This way, the caller of the create() function of the subclass can use the returned
 * instance as VariableRequirement. This allows for generic handling of variable requirements for different variables.
 *
 * Example create() function of the subclass:
 * ```
 * VariableRequirement my_requirement_create(uint8_t alert_id, uint8_t operator, MyValueType requirement_value)
 * {
 *     MyRequirement self = variable_requirement_allocator_alloc();
 *     variable_requirement_create((VariableRequirement)self, &interface, operator, alert_id);
 *
 *     self->value = value;
 *     return (VariableRequirement)self;
 * }
 * ```
 */

/**
 * @brief Evaluate variable requirement.
 *
 * @param self Variable requirement instance returned by the create() function of one of the subclasses of
 * VariableRequirement.
 *
 * @return true Variable requirement evaluated to true.
 * @return false Variable requirement evaluated to false.
 */
bool variable_requirement_evaluate(VariableRequirement self);

/**
 * @brief Check whether the result of the variable requirement changed from the last evaluation to second to last
 * evaluation.
 *
 * @pre @ref variable_requirement_evaluate has been called at least once.
 *
 * @param self Variable requirement instance returned by the create() function of one of the subclasses of
 * VariableRequirement.
 *
 * @return true The last two calls to @ref variable_requirement_evaluate returned the same values, or @ref
 * variable_requirement_evaluate has been called only once so far.
 * @return false The last two calls to @ref variable_requirement_evaluate returned different values.
 */
bool variable_requirement_is_result_changed(VariableRequirement self);

/**
 * @brief Get alert id of the alert to which this variable requirement belongs.
 *
 * @param self Variable requirement instance returned by the create() function of one of the subclasses of
 * VariableRequirement.
 *
 * @return uint8_t Alert id.
 */
uint8_t variable_requirement_get_alert_id(VariableRequirement self);

/**
 * @brief Destroy variable requirement.
 *
 * @param self Variable requirement instance returned by the create() function of one of the subclasses of
 * VariableRequirement.
 */
void variable_requirement_destroy(VariableRequirement self);

#ifdef __cplusplus
}
#endif

#endif
