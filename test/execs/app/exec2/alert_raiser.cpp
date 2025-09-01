#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "alert_raiser.h"

TEST_GROUP(AlertRaiser){};

TEST(AlertRaiser, Warmup0ImmediatelyNotifiesWhenAlertConditionBecomesTrue)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period = 0;
    uint32_t cooldown_period = 0;
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period, cooldown_period);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}

TEST(AlertRaiser, Warmup0Cooldown0ImmediatelyNotify)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period = 0;
    uint32_t cooldown_period = 0;
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period, cooldown_period);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}
