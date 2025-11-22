#include "CppUTest/TestHarness.h"

#include "alert_evaluation_readiness.h"

// clang-format off
TEST_GROUP(AlertEvaluationReadiness){
    void setup()
    {
        alert_evaluation_readiness_reset();
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
