#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "alert_raiser.h"
#include "eas_timer_defs.h"

#define TEST_ALERT_RAISER_MAX_NUM_TIMER_CBS 2

/* The implementation of eas_timer_create in EasTimer mock object populates these with the warmup and cooldown
 * callbacks and their user data. This is needed in the test so that we can call these callback to simulate
 * warmup/cooldown periods expiring. */
static EasTimerCb timer_cbs[TEST_ALERT_RAISER_MAX_NUM_TIMER_CBS];
static void *timer_cbs_user_data[TEST_ALERT_RAISER_MAX_NUM_TIMER_CBS];

static EasTimer warmup_timer = (EasTimer)0x42;
static EasTimer cooldown_timer = (EasTimer)0x43;

// clang-format off
TEST_GROUP(AlertRaiser)
{
    void setup() {
        /* Pass pointers and arrays to the mock object so that it populates them with warmup/cooldown callbacks and their user data*/
        mock().setData("timerCbs", (void *)timer_cbs);
        mock().setData("timerCbsUserData", timer_cbs_user_data);
        /* Give the size of the arrays to the mock object so that it never does out of bound accesses */
        mock().setData("numTimerCbs", (unsigned int)TEST_ALERT_RAISER_MAX_NUM_TIMER_CBS);
    }
};
// clang-format on

TEST(AlertRaiser, Warmup0ImmediatelyNotifiesWhenAlertConditionBecomesTrue)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}

TEST(AlertRaiser, Warmup0Cooldown0ImmediatelyNotify)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}

TEST(AlertRaiser, WarmupNot0Cooldown0)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 1000;
    uint32_t cooldown_period_ms = 0;

    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    /* alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    /* Inside warmup callback */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);

    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Call the warmup callback to simulate the warmup period expiring */
    warmup_cb(warmup_cb_user_data);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}

TEST(AlertRaiser, Warmup0CooldownNot0)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 1000;

    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);
    /* alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Inside cooldown callback */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);

    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Call the cooldown callback to simulate the cooldown period expiring */
    cooldown_cb(cooldown_cb_user_data);
}

TEST(AlertRaiser, Warmup0Cooldown0ConditionResultSetSeveralTimes)
{
    uint8_t alert_id = 1;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;

    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* First three calls to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* First three calls to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);
    /* Second two calls to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* Second two calls to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);
    /* Last call to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}

TEST(AlertRaiser, WarmupNot0CooldownNot0ConditionResultSetSeveralTimes)
{
    uint8_t alert_id = 99;
    uint32_t warmup_period_ms = 2002;
    uint32_t cooldown_period_ms = 10001;

    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);
    /* First four calls to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    /* Warmup cb 1 */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* First three calls to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Cooldown cb 1 */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);
    /* Fifth call to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    /* Warmup cb 2 */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* Fifth call to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Cooldown cb 2 */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);

    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    warmup_cb(warmup_cb_user_data);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    cooldown_cb(cooldown_cb_user_data);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    warmup_cb(warmup_cb_user_data);
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    cooldown_cb(cooldown_cb_user_data);
}

TEST(AlertRaiser, AlertConditionSetBeforeTimerExpires)
{
    uint8_t alert_id = 24;
    uint32_t warmup_period_ms = 5000;
    uint32_t cooldown_period_ms = 3000;

    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* alert_raiser_set_alert */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);
    /* First call to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    /* Two calls to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("eas_timer_stop").withParameter("self", warmup_timer);
    /* Second and third calls to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    /* Warmup cb */
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* Third call to alert_raiser_set_alert_condition_result(false) */
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Fourth call to alert_raiser_set_alert_condition_result(true) */
    mock().expectOneCall("eas_timer_stop").withParameter("self", cooldown_timer);

    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* This will start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Condition result changed to true before warmup timer expired. This should stop the warmup timer. */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* The second call to alert_raiser_set_alert_condition_result(false) here checks that the call to eas_timer_stop
     * happens only once. Nothing should happen as a result of this call. */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* This start the warmup timer again*/
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* The second call to alert_raiser_set_alert_condition_result(true) here checks that the call to eas_timer_start
     * happens only once */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Simulate warmup timer expiring. Should call alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* This will start the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Condition result changed to true before cooldown timer expired. This should stop the cooldown timer. */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* The alert is now raised, so these calls should have no effect */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}
