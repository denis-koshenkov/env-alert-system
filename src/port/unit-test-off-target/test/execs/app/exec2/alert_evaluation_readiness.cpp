#include "CppUTest/TestHarness.h"

#include "alert_evaluation_readiness.h"

TEST_GROUP(AlertEvaluationReadiness){};

TEST(AlertEvaluationReadiness, IsReadyInitiallyFalse)
{
    bool is_ready = alert_evaluation_readiness_is_ready();
    CHECK_FALSE(is_ready);
}
