#ifndef VARIABLE_REQUIREMENTS_MOCK_VARIABLE_REQUIREMENT_H
#define VARIABLE_REQUIREMENTS_MOCK_VARIABLE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

/* For pass_null_instance_to_var_req_create parameter of mock_variable_requirement_create() */
#define MOCK_VARIABLE_REQUIREMENT_PASS_NULLPTR_INSTANCE_TO_VAR_REQ_CREATE true
#define MOCK_VARIABLE_REQUIREMENT_PASS_VALID_INSTANCE_TO_VAR_REQ_CREATE false

/* For pass_invalid_operator_to_var_req_create parameter of mock_variable_requirement_create() */
#define MOCK_VARIABLE_REQUIREMENT_PASS_INVALID_OPERATOR_TO_VAR_REQ_CREATE true
#define MOCK_VARIABLE_REQUIREMENT_PASS_GEQ_OPERATOR_TO_VAR_REQ_CREATE false

/* For vtable_parameter parameter of mock_variable_requirement_create() */
typedef enum MockVariableRequirementVtableParameter {
    MOCK_VARIABLE_REQUIREMENT_PASS_NULL_VTABLE_TO_VAR_REQ_CREATE,
    MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE,
    MOCK_VARIABLE_REQUIREMENT_PASS_NULL_EVALUATE_TO_VAR_REQ_CREATE,
    MOCK_VARIABLE_REQUIREMENT_PASS_NULL_DESTROY_TO_VAR_REQ_CREATE
} MockVariableRequirementVtableParameter;

/**
 * @brief Create mock variable requirement.
 *
 * @param pass_null_instance_to_var_req_create If true, when calling variable_requirement_create(), passes a NULL
 * pointer argument to the VariableRequirement instance parameter. If false, passes the proper memory returned by
 * variable_requirement_allocator_alloc().
 * @param pass_invalid_operator_to_var_req_create If true, when calling variable_requirement_create(), passes
 * VARIABLE_REQUIREMENT_OPERATOR_INVALID to the VariableRequirementOperator parameter. If false, passes a valid
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ operator to that parameter.
 * @param vtable_parameter Use one of the values from @ref MockVariableRequirementVtableParameter. Defines what kind of
 * argument is passed to the vtable parameter of variable_requirement_create():
 *   - MOCK_VARIABLE_REQUIREMENT_PASS_NULL_VTABLE_TO_VAR_REQ_CREATE - NULL pointer.
 *   - MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE - valid vtable with a valid implementation of
 * evaluate().
 *   - MOCK_VARIABLE_REQUIREMENT_PASS_NULL_EVALUATE_TO_VAR_REQ_CREATE - vtable that has a NULL pointer instead of the
 * evaluate() implementation.
 *
 * @return VariableRequirement Created variable requirement instance.
 */
VariableRequirement mock_variable_requirement_create(bool pass_null_instance_to_var_req_create,
                                                     bool pass_invalid_operator_to_var_req_create,
                                                     uint8_t vtable_parameter);

void mock_variable_requirement_destroy(VariableRequirement mock_variable_requirement);

/**
 * @brief Set evaluate result for mock variable requirement.
 *
 * By calling this function, the test can control the return value of all subsequent calls to
 * variable_requirement_evaluate() of the provided mock variable requirement instance.
 *
 * All subsequent calls to variable_requirement_evaluate() will keep returning @p result , until this function is called
 * again to set a different result.
 *
 * @param mock_variable_requirement Mock variable requirement instance.
 * @param result Result to set.
 */
void mock_variable_requirement_set_evaluate_result(VariableRequirement mock_variable_requirement, bool result);

#ifdef __cplusplus
}
#endif

#endif
