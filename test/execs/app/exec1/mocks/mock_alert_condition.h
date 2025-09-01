#ifndef MOCK_ALERT_CONDITION_H
#define MOCK_ALERT_CONDITION_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct AlertConditionStruct *AlertCondition;

AlertCondition alert_condition_create();

#ifdef __cplusplus
}
#endif

#endif
