#include <stddef.h>

#include "alert_raisers.h"
#include "alert_raiser.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE
#define CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE 1
#endif

static AlertRaiser alert_raiser_instances[CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE];

void alert_raisers_create_instances()
{
    for (size_t i = 0; i < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE; i++) {
        if (alert_raiser_instances[i] != NULL) {
            /* This function has already been called before - instances are already created */
            EAS_ASSERT(0);
        }
    }

    for (size_t i = 0; i < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE; i++) {
        alert_raiser_instances[i] = alert_raiser_create();
    }
}

AlertRaiser alert_raisers_get_alert_raiser(uint8_t alert_id)
{
    bool is_valid_alert_id = (alert_id < CONFIG_ALERT_RAISERS_NUM_INSTANCES_TO_CREATE);
    EAS_ASSERT(is_valid_alert_id);
    AlertRaiser alert_raiser = alert_raiser_instances[alert_id];
    /* This assert will fail if alert_raisers_create_instances has not been called yet. Since
     * the alert_raiser_instances array is a static global, it is guaranteed to be initialized to 0.*/
    EAS_ASSERT(alert_raiser);

    return alert_raiser;
}
