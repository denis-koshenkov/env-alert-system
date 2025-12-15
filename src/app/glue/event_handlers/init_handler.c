#include "init_handler.h"
#include "alert_conditions.h"
#include "alert_raisers.h"
#include "alert_adder.h"
#include "alert_remover.h"
#include "msg_transceiver.h"

void init_handler_handle_init_event()
{
    alert_conditions_create_instances();
    alert_raisers_create_instances();
    msg_transceiver_init();
    msg_transceiver_set_add_alert_cb(alert_adder_add_alert, NULL);
    msg_transceiver_set_remove_alert_cb(alert_remover_remove_alert, NULL);
}
