#include <stddef.h>

#include "alert_conditions.h"
#include "eas_assert.h"

static AlertCondition alert_condition_instance = NULL;

void alert_conditions_set_alert_condition(uint8_t alert_id, AlertCondition alert_condition)
{
    EAS_ASSERT(alert_condition);
    alert_condition_instance = alert_condition;
}

AlertCondition alert_conditions_get_alert_condition(uint8_t alert_id)
{
    EAS_ASSERT(alert_condition_instance);
    return alert_condition_instance;
}
