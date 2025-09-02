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
    /* Call the cooldown callback to simulate the warmup period expiring */
    cooldown_cb(cooldown_cb_user_data);
}
