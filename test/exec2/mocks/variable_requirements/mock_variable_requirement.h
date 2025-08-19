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

/* For pass_null_vtable parameter of mock_variable_requirement_create() */
#define MOCK_VARIABLE_REQUIREMENT_PASS_NULL_VTABLE_TO_VAR_REQ_CREATE true
#define MOCK_VARIABLE_REQUIREMENT_PASS_VALID_VTABLE_TO_VAR_REQ_CREATE false

/**
 * @brief Create mock variable requirement.
 *
 * @param pass_null_instance_to_var_req_create If true, when calling variable_requirement_create(), passes a NULL
 * pointer argument to the VariableRequirement instance parameter. If false, passes the proper memory returned by
 * variable_requirement_allocator_alloc().
 * @param pass_invalid_operator_to_var_req_create If true, when calling variable_requirement_create(), passes
 * VARIABLE_REQUIREMENT_OPERATOR_INVALID to the VariableRequirementOperator parameter. If false, passes a valid
 * VARIABLE_REQUIREMENT_OPERATOR_GEQ operator to that parameter.
 * @param pass_null_vtable If true, when calling variable_requirement_create(), passes NULL to the vtable parameter. If
 * false, passes a valid interface to that parameter.
 *
 * @return VariableRequirement Created variable requirement instance.
 */
VariableRequirement mock_variable_requirement_create(bool pass_null_instance_to_var_req_create,
                                                     bool pass_invalid_operator_to_var_req_create,
                                                     bool pass_null_vtable);

void mock_variable_requirement_destroy(VariableRequirement mock_variable_requirement);

void mock_variable_requirement_set_evaluate_result(VariableRequirement mock_variable_requirement, bool result);

#ifdef __cplusplus
}
#endif

#endif
