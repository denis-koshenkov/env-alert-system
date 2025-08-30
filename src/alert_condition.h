#ifndef ALERT_CONDITION_H
#define ALERT_CONDITION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

typedef struct AlertConditionStruct *AlertCondition;

AlertCondition alert_condition_create();

bool alert_condition_evaluate(AlertCondition self);

#ifdef __cplusplus
}
#endif

#endif
