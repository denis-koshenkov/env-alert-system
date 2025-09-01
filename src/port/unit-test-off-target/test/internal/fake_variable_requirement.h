#ifndef ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_INTERNAL_FAKE_VARIABLE_REQUIREMENT_H
#define ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_INTERNAL_FAKE_VARIABLE_REQUIREMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "variable_requirement.h"

/**
 * @brief Create fake variable requirement.
 *
 * @return VariableRequirement Created fake variable requirement instance.
 */
VariableRequirement fake_variable_requirement_create();

/**
 * @brief Set evaluate result for fake variable requirement.
 *
 * By calling this function, the test can control the return value of all subsequent calls to
 * variable_requirement_evaluate() of the provided fake variable requirement instance.
 *
 * All subsequent calls to variable_requirement_evaluate() will keep returning @p result, until this function is called
 * again to set a different result.
 *
 * @param fake_variable_requirement Fake variable requirement instance returned by @ref
 * fake_variable_requirement_create.
 * @param result Result to set.
 */
void fake_variable_requirement_set_evaluate_result(VariableRequirement fake_variable_requirement, bool result);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_PORT_UNIT_TEST_OFF_TARGET_TEST_INTERNAL_FAKE_VARIABLE_REQUIREMENT_H */
