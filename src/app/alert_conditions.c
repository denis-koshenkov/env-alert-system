#include <stddef.h>

#include "alert_conditions.h"
#include "alert_condition.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_MAX_NUM_ALERTS
#define CONFIG_MAX_NUM_ALERTS 1
#endif

static AlertCondition alert_condition_instances[CONFIG_MAX_NUM_ALERTS];

void alert_conditions_create_instances()
{
    for (size_t i = 0; i < CONFIG_MAX_NUM_ALERTS; i++) {
        if (alert_condition_instances[i] != NULL) {
            /* This function has already been called before - instances are already created */
            EAS_ASSERT(0);
        }
    }

    for (size_t i = 0; i < CONFIG_MAX_NUM_ALERTS; i++) {
        alert_condition_instances[i] = alert_condition_create();
    }
}

AlertCondition alert_conditions_get_alert_condition(uint8_t alert_id)
{
    bool is_valid_alert_id = (alert_id < CONFIG_MAX_NUM_ALERTS);
    EAS_ASSERT(is_valid_alert_id);
    AlertCondition alert_condition = alert_condition_instances[alert_id];
    /* This assert will fail if alert_conditions_create_instances has not been called yet. Since
     * the alert_condition_instances array is a static global, it is guaranteed to be initialized to 0.*/
    EAS_ASSERT(alert_condition);

    return alert_condition;
}
