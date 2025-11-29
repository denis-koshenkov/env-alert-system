#include "alert_remover.h"
#include "alert_validator.h"
#include "alert_raisers.h"
#include "connectivity_notifier.h"
#include "alert_raiser.h"
#include "led_notifier.h"
#include "temperature_requirement_list.h"
#include "pressure_requirement_list.h"
#include "humidity_requirement_list.h"
#include "light_intensity_requirement_list.h"
#include "alert_conditions.h"
#include "alert_condition.h"
#include "variable_requirement.h"

static void destroy_variable_requirement(VariableRequirement variable_requirement)
{
    variable_requirement_destroy(variable_requirement);
}

void alert_remover_remove_alert(uint8_t alert_id)
{
    /* Do not attempt to remove an alert with an invalid id */
    if (!alert_validator_is_alert_id_valid(alert_id)) {
        return;
    }

    AlertRaiser alert_raiser = alert_raisers_get_alert_raiser(alert_id);
    if (!alert_raiser_is_alert_set(alert_raiser)) {
        /* Alert with this id does not exist - nothing to remove */
        return;
    }

    /* If the alert to be removed is currently raised, we do not want to send a connectivity notification that it is now
     * silenced, because the alert is being removed anyway. This is why we disable connectivity notifications for this
     * alert before doing anything else. */
    connectivity_notifier_disable_notifications(alert_id);

    /* This will silence the alert if it is currently raised, and then unset it. Since we already disabled connectivity
     * notifications above, we will not send a notification that an alert is now silenced. Silencing an alert before
     * removing it is necessary to stop displaying the led notification, if one is being displayed. */
    alert_raiser_unset_alert(alert_raiser);

    led_notifier_disable_notifications(alert_id);

    /* Remove variable requirements of this alert from all variable requirement lists */
    temperature_requirement_list_remove_all_for_alert(alert_id);
    pressure_requirement_list_remove_all_for_alert(alert_id);
    humidity_requirement_list_remove_all_for_alert(alert_id);
    light_intensity_requirement_list_remove_all_for_alert(alert_id);

    /* Destroy all variable requirement instances that belong to this alert */
    AlertCondition alert_condition = alert_conditions_get_alert_condition(alert_id);
    alert_condition_for_each(alert_condition, destroy_variable_requirement);

    /* Not strictly necessary, since this is called by alert_adder before adding anything to the alert condition, but it
     * is nice to clean everything up here as soon as the alert gets removed. */
    alert_condition_reset(alert_condition);
}
