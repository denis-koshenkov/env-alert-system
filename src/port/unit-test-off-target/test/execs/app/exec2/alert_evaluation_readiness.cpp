#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "alert_evaluation_readiness.h"

size_t ready_cb_call_count = 0;

static void ready_cb()
{
    ready_cb_call_count++;
}

// clang-format off
TEST_GROUP(AlertEvaluationReadiness){
    void setup()
    {
        alert_evaluation_readiness_reset();
        ready_cb_call_count = 0;
    }
};
// clang-format on

TEST(AlertEvaluationReadiness, IsReadyInitiallyFalse)
{
    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_FALSE(is_ready);
}

TEST(AlertEvaluationReadiness, IsReadyFalseAfterSomeButNotAllSamples1)
{
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();

    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_FALSE(is_ready);
}

TEST(AlertEvaluationReadiness, IsReadyFalseAfterSomeButNotAllSamples2)
{
    alert_evaluation_readiness_notify_received_light_intensity_sample();

    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_FALSE(is_ready);
}

TEST(AlertEvaluationReadiness, IsReadyFalseAfterSomeButNotAllSamples3)
{
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();

    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_FALSE(is_ready);
}

TEST(AlertEvaluationReadiness, IsReadyTrueAfterAllSamplesReceived)
{
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();

    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_TRUE(is_ready);
}

TEST(AlertEvaluationReadiness, IsReadyTrueAfterAllSamplesReceivedMultipleCalls)
{
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    bool is_ready_1 = alert_evaluation_readiness_is_ready();

    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    bool is_ready_2 = alert_evaluation_readiness_is_ready();

    CHECK_TRUE(is_ready_1);
    CHECK_TRUE(is_ready_2);
}

TEST(AlertEvaluationReadiness, CbExecutedWhenIsReadyBecomesTrueTemperature)
{
    alert_evaluation_readiness_set_ready_cb(ready_cb);

    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();

    CHECK_EQUAL(1, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbExecutedWhenIsReadyBecomesTruePressure)
{
    alert_evaluation_readiness_set_ready_cb(ready_cb);

    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();

    CHECK_EQUAL(1, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbExecutedWhenIsReadyBecomesTrueHumidity)
{
    alert_evaluation_readiness_set_ready_cb(ready_cb);

    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();

    CHECK_EQUAL(1, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbExecutedWhenIsReadyBecomesTrueLightIntensity)
{
    alert_evaluation_readiness_set_ready_cb(ready_cb);

    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();

    CHECK_EQUAL(1, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbNotExecutedWhenNotSetTemperature)
{
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();

    CHECK_EQUAL(0, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbNotExecutedWhenNotSetPressure)
{
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();

    CHECK_EQUAL(0, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbNotExecutedWhenNotSetHumidity)
{
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();

    CHECK_EQUAL(0, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, CbNotExecutedWhenNotSetLightIntensity)
{
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();

    CHECK_EQUAL(0, ready_cb_call_count);
}

TEST(AlertEvaluationReadiness, ResetClearsReadyCb)
{
    alert_evaluation_readiness_set_ready_cb(ready_cb);
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    /* Should be 1 - callback was supposed to execute when we received samples for each variable */
    size_t ready_cb_call_count_1 = ready_cb_call_count;

    alert_evaluation_readiness_reset();
    alert_evaluation_readiness_notify_received_temperature_sample();
    alert_evaluation_readiness_notify_received_pressure_sample();
    alert_evaluation_readiness_notify_received_humidity_sample();
    alert_evaluation_readiness_notify_received_light_intensity_sample();
    /* Should still be 1 (not 2) since we did not register a ready callback after calling reset() */
    size_t ready_cb_call_count_2 = ready_cb_call_count;

    CHECK_EQUAL(1, ready_cb_call_count_1);
    CHECK_EQUAL(1, ready_cb_call_count_2);
}

TEST(AlertEvaluationReadiness, SetReadyCbAssertsIfCbNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("cb", "alert_evaluation_readiness_set_ready_cb");

    alert_evaluation_readiness_set_ready_cb(NULL);
}
