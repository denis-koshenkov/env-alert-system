#ifndef ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_DEFS_H
#define ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Alert condition public definitions.
 *
 * The definition of the AlertCondition type is here, so that other modules can use it without including the whole
 * alert_condition.h header.
 *
 * For example, the alert conditions module only needs to know about the AlertCondition type definition, it should
 * not know about and expose the whole public API defined in alert_condition.h.
 */

typedef struct AlertConditionStruct *AlertCondition;

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_ALERT_CONDITION_DEFS_H */
