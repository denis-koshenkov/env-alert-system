#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

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
        /* Order of expected calls is important for these tests. Fail the test if the expected mock calls do not happen
        in the specified order */
        mock().strictOrder();

        /* Pass pointers and arrays to the mock object so that it populates them with warmup/cooldown callbacks and their user data */
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
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
    /* Condition result changed to false before warmup timer expired. This should stop the warmup timer. */
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

TEST(AlertRaiser, WarmupTimerCbExecutedAfterTimerStopped)
{
    uint8_t alert_id = 42;
    uint32_t warmup_period_ms = 10000;
    uint32_t cooldown_period_ms = 0;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock().expectOneCall("eas_timer_stop").withParameter("self", warmup_timer);
    /* Do not expect the call to alert_notifier_notify(true) from within the warmup timer cb */

    /* Creates warmup and cooldown timer instances*/
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    /* Sets the period of the warmup timer */
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* This will start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Condition result changed to true before warmup timer expired. This should stop the warmup timer. */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Even though the timer is stopped, the warmup timer callback is still executed. Inside the callback, the alert
     * should not be raised, since the alert condition was already set to false. */
    warmup_cb(warmup_cb_user_data);
}

TEST(AlertRaiser, CooldownTimerCbExecutedAfterTimerStopped)
{
    uint8_t alert_id = 9;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 20000;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    mock().expectOneCall("eas_timer_stop").withParameter("self", cooldown_timer);
    /* Do not expect the call to alert_notifier_notify(false) from within the cooldown timer cb */

    /* Creates warmup and cooldown timer instances*/
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets the period of the cooldown timer */
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* This will raise the alert immediately since the warmup period is 0 */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* This will start the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Cooldown timer has not expired yet, but the alert condition result changed to true. This will stop the cooldown
     * timer. */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Cooldown callback gets executed even though cooldown timer is stopped. The alert raiser should not call
     * alert_notifier_notify(false), because the alert condition was already set to true. */
    cooldown_cb(cooldown_cb_user_data);
}

TEST(AlertRaiser, SetAlertConditionTrueAfterWarmupExpiredHasNoEffect)
{
    uint8_t alert_id = 3;
    uint32_t warmup_period_ms = 5000;
    uint32_t cooldown_period_ms = 0;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    /* Do not expect another alert_notifier_notify(true) call */

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* This call has no effect since the alert is already raised */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}

TEST(AlertRaiser, SetAlertConditionTrueAfterCooldownExpiredHasNoEffect)
{
    uint8_t alert_id = 4;
    uint32_t warmup_period_ms = 4321;
    uint32_t cooldown_period_ms = 1234;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    mock().expectOneCall("alert_notifier_notify").withParameter("alert_id", alert_id).withParameter("is_raised", false);
    /* Do not expect another alert_notifier_notify(false) call */

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Silences the alert - calls alert_notifier_notify(false) */
    cooldown_cb(cooldown_cb_user_data);
    /* This call has no effect since the alert is already silenced */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}

TEST(AlertRaiser, AlertIdAndTimerPeriodsAreOverwrittenWhenNewAlertIsSet)
{
    uint8_t alert_0_id = 5;
    uint32_t alert_0_warmup_period_ms = 300;
    uint32_t alert_0_cooldown_period_ms = 200;

    uint8_t alert_1_id = 10;
    uint32_t alert_1_warmup_period_ms = 0;
    uint32_t alert_1_cooldown_period_ms = 0;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* Expected calls for alert 0 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_0_warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", alert_0_cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", false);
    /* Expected calls for alert 1 */
    /* No expected calls to eas_timer_set_period, since both warmup and cooldown periods are 0 */
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", true);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", false);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Silences the alert - calls alert_notifier_notify(false) */
    cooldown_cb(cooldown_cb_user_data);
    /* Alert is already silenced and cooldown timer is not running - we do not expect any calls from this function */
    alert_raiser_unset_alert(alert_raiser);

    /* Set a new alert */
    alert_raiser_set_alert(alert_raiser, alert_1_id, alert_1_warmup_period_ms, alert_1_cooldown_period_ms);
    /* Raises alert - warmup period is 0 */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Silences alert - warmup period is 1 */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}

TEST(AlertRaiser, CooldownTimerRunningWhenNewAlarmIsSet)
{
    uint8_t alert_0_id = 6;
    uint32_t alert_0_warmup_period_ms = 500;
    uint32_t alert_0_cooldown_period_ms = 600;

    uint8_t alert_1_id = 255;
    uint32_t alert_1_warmup_period_ms = 1;
    uint32_t alert_1_cooldown_period_ms = 2;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* Expected calls for alert 0 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_0_warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", alert_0_cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Expected calls from unset_alert */
    mock().expectOneCall("eas_timer_stop").withParameter("self", cooldown_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", false);
    /* Expected calls for alert 1 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_1_warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", alert_1_cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", false);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Alert is currently raised and the cooldown timer is running. This call should stop the cooldown timer, and
     * silence the alert. */
    alert_raiser_unset_alert(alert_raiser);

    /* This call should set warmup and cooldown periods for the new alert. */
    alert_raiser_set_alert(alert_raiser, alert_1_id, alert_1_warmup_period_ms, alert_1_cooldown_period_ms);
    /* Cooldown cb for the old alert still gets executed - this should have no effect */
    cooldown_cb(cooldown_cb_user_data);
    /* New alert should be initially silenced - this call should have no effect */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Starts the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Silences the alert - calls alert_notifier_notify(false) */
    cooldown_cb(cooldown_cb_user_data);
}

TEST(AlertRaiser, UnsetAlertStopsCooldownTimer)
{
    uint8_t alert_0_id = 8;
    uint32_t alert_0_warmup_period_ms = 950;
    uint32_t alert_0_cooldown_period_ms = 430;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* Expected calls for alert 0 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_0_warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", alert_0_cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    /* Expected calls from unset_alert */
    mock().expectOneCall("eas_timer_stop").withParameter("self", cooldown_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_0_id)
        .withParameter("is_raised", false);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Alert is currently raised and the cooldown timer is running. This call should stop the cooldown timer, and
     * silence the alert. */
    alert_raiser_unset_alert(alert_raiser);
}

TEST(AlertRaiser, WarmupTimerRunningWhenAlertIsUnset)
{
    uint8_t alert_0_id = 24;
    uint32_t alert_0_warmup_period_ms = 777;
    uint32_t alert_0_cooldown_period_ms = 0;

    uint8_t alert_1_id = 45;
    uint32_t alert_1_warmup_period_ms = 99;
    uint32_t alert_1_cooldown_period_ms = 98;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    /* Expected calls for alert 0 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_0_warmup_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock().expectOneCall("eas_timer_stop").withParameter("self", warmup_timer);
    /* Expected calls for alert 1 */
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_1_warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", alert_1_cooldown_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", true);
    mock().expectOneCall("eas_timer_start").withParameter("self", cooldown_timer);
    mock()
        .expectOneCall("alert_notifier_notify")
        .withParameter("alert_id", alert_1_id)
        .withParameter("is_raised", false);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Unset alert, this should stop the currently running warmup timer */
    alert_raiser_unset_alert(alert_raiser);

    /* This call should set warmup and cooldown periods for the new alert. */
    alert_raiser_set_alert(alert_raiser, alert_1_id, alert_1_warmup_period_ms, alert_1_cooldown_period_ms);
    /* Warmup cb for the old alert still gets executed - this should have no effect */
    warmup_cb(warmup_cb_user_data);
    /* New alert should be initially silenced - this call should have no effect */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Starts the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Raises the alert - calls alert_notifier_notify(true) */
    warmup_cb(warmup_cb_user_data);
    /* Starts the cooldown timer */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
    /* Silences the alert - calls alert_notifier_notify(false) */
    cooldown_cb(cooldown_cb_user_data);
}

TEST(AlertRaiser, UnsetAlertStopsWarmupTimer)
{
    uint8_t alert_0_id = 19;
    uint32_t alert_0_warmup_period_ms = 666;
    uint32_t alert_0_cooldown_period_ms = 0;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", alert_0_warmup_period_ms);
    mock().expectOneCall("eas_timer_start").withParameter("self", warmup_timer);
    mock().expectOneCall("eas_timer_stop").withParameter("self", warmup_timer);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    EasTimerCb warmup_cb = timer_cbs[0];
    void *warmup_cb_user_data = timer_cbs_user_data[0];
    EasTimerCb cooldown_cb = timer_cbs[1];
    void *cooldown_cb_user_data = timer_cbs_user_data[1];
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    /* Start the warmup timer */
    alert_raiser_set_alert_condition_result(alert_raiser, true);
    /* Unset alert, this should stop the currently running warmup timer */
    alert_raiser_unset_alert(alert_raiser);
}

TEST(AlertRaiser, SetAlertConditionAssertsIfCalledBeforeSetAlert)
{
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self->is_alert_set", "alert_raiser_set_alert_condition_result");

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert_condition_result(alert_raiser, true);
}

TEST(AlertRaiser, AlertIsInitiallySilenced)
{
    uint8_t alert_id = 12;
    uint32_t warmup_period_ms = 13;
    uint32_t cooldown_period_ms = 14;
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", warmup_timer)
        .withParameter("period_ms", warmup_period_ms);
    mock()
        .expectOneCall("eas_timer_set_period")
        .withParameter("self", cooldown_timer)
        .withParameter("period_ms", cooldown_period_ms);

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    /* Sets timer period */
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* This call should have no effect - cooldown timer should not be started, since the alert is initially silenced. */
    alert_raiser_set_alert_condition_result(alert_raiser, false);
}

TEST(AlertRaiser, IsAlertSetIsInitiallyFalse)
{
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);

    AlertRaiser alert_raiser = alert_raiser_create();
    bool is_alert_set = alert_raiser_is_alert_set(alert_raiser);
    CHECK_FALSE(is_alert_set);
}

TEST(AlertRaiser, IsAlertSetTrueAfterAlertIsSet)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    bool is_alert_set = alert_raiser_is_alert_set(alert_raiser);
    CHECK_TRUE(is_alert_set);
}

TEST(AlertRaiser, IsAlertSetFalseAfterAlertIsSetAndUnset)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    bool is_alert_set_after_set = alert_raiser_is_alert_set(alert_raiser);
    alert_raiser_unset_alert(alert_raiser);
    bool is_alert_set_after_unset = alert_raiser_is_alert_set(alert_raiser);

    CHECK_TRUE(is_alert_set_after_set);
    CHECK_FALSE(is_alert_set_after_unset);
}

TEST(AlertRaiser, UnsetAlertHasNoEffectWhenAlertIsNotSet)
{
    uint8_t alert_id = 5;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);

    AlertRaiser alert_raiser = alert_raiser_create();
    /* No alert has ever been set, should be false*/
    bool is_alert_set_1 = alert_raiser_is_alert_set(alert_raiser);
    alert_raiser_unset_alert(alert_raiser);
    bool is_alert_set_2 = alert_raiser_is_alert_set(alert_raiser);
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    /* Should be true since alert is now set*/
    bool is_alert_set_3 = alert_raiser_is_alert_set(alert_raiser);
    alert_raiser_unset_alert(alert_raiser);
    /* Should be false since alert is now unset */
    bool is_alert_set_4 = alert_raiser_is_alert_set(alert_raiser);
    alert_raiser_unset_alert(alert_raiser);
    bool is_alert_set_5 = alert_raiser_is_alert_set(alert_raiser);

    CHECK_FALSE(is_alert_set_1);
    CHECK_FALSE(is_alert_set_2);
    CHECK_TRUE(is_alert_set_3);
    CHECK_FALSE(is_alert_set_4);
    CHECK_FALSE(is_alert_set_5);
}

TEST(AlertRaiser, SetAlertAssertsIfUnsetIsNotCalled)
{
    uint8_t alert_0_id = 9;
    uint32_t alert_0_warmup_period_ms = 0;
    uint32_t alert_0_cooldown_period_ms = 0;

    uint8_t alert_1_id = 11;
    uint32_t alert_1_warmup_period_ms = 0;
    uint32_t alert_1_cooldown_period_ms = 0;
    /* alert_raiser_create */
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("!self->is_alert_set", "alert_raiser_set_alert");

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_0_id, alert_0_warmup_period_ms, alert_0_cooldown_period_ms);
    alert_raiser_set_alert(alert_raiser, alert_1_id, alert_1_warmup_period_ms, alert_1_cooldown_period_ms);
}

TEST(AlertRaiser, SetAlertAssertsIfInstanceNull)
{
    uint8_t alert_id = 0;
    uint32_t warmup_period_ms = 132;
    uint32_t cooldown_period_ms = 267;

    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "alert_raiser_set_alert");

    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(NULL, alert_id, warmup_period_ms, cooldown_period_ms);
}

TEST(AlertRaiser, SetAlertConditionResultAssertsIfInstanceNull)
{
    uint8_t alert_id = 57;
    uint32_t warmup_period_ms = 0;
    uint32_t cooldown_period_ms = 0;
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(warmup_timer);
    mock()
        .expectOneCall("eas_timer_create")
        .withParameter("period_ms", 0)
        .withParameter("periodic", false)
        .ignoreOtherParameters()
        .andReturnValue(cooldown_timer);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "alert_raiser_set_alert_condition_result");

    /* Creates warmup and cooldown timer instances */
    AlertRaiser alert_raiser = alert_raiser_create();
    alert_raiser_set_alert(alert_raiser, alert_id, warmup_period_ms, cooldown_period_ms);
    alert_raiser_set_alert_condition_result(NULL, true);
}
