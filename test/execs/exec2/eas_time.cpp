#include "CppUTest/TestHarness.h"

#include "utils/eas_time.h"

TEST_GROUP(EasTime){};

TEST(EasTime, OffsetIntoFuture0StaysTheSame20)
{
    EasTime initial_time = 20;
    EasTime time_with_offset = eas_time_offset_into_future(initial_time, 0);
    CHECK_EQUAL(initial_time, time_with_offset);
}

TEST(EasTime, OffsetIntoFuture0StaysTheSame0)
{
    EasTime initial_time = 0;
    EasTime time_with_offset = eas_time_offset_into_future(initial_time, 0);
    CHECK_EQUAL(initial_time, time_with_offset);
}

TEST(EasTime, OffsetIntoFuture10IsAdded)
{
    EasTime time_with_offset = eas_time_offset_into_future(100, 10);
    CHECK_EQUAL(110, time_with_offset);
}

TEST(EasTime, OffsetIntoFutureWithOverflowResultZero)
{
    EasTime time_with_offset = eas_time_offset_into_future(0xFFFFFFF0, 0x10);
    CHECK_EQUAL(0, time_with_offset);
}

TEST(EasTime, OffsetIntoFutureWithOverflowResultPositive)
{
    EasTime time_with_offset = eas_time_offset_into_future(0xFFFFFF00, 0x200);
    CHECK_EQUAL(0x100, time_with_offset);
}

TEST(EasTime, IsEqualOrAfter0and0True)
{
    bool result = eas_time_is_equal_or_after(0, 0);
    CHECK_EQUAL(true, result);
}

TEST(EasTime, IsEqualOrAfter2and2True)
{
    bool result = eas_time_is_equal_or_after(2, 2);
    CHECK_EQUAL(true, result);
}

TEST(EasTime, IsEqualOrAfter100and200False)
{
    bool result = eas_time_is_equal_or_after(100, 200);
    CHECK_EQUAL(false, result);
}

TEST(EasTime, IsEqualOrAfterMaxand0False)
{
    bool result = eas_time_is_equal_or_after(0xFFFFFFFF, 0);
    CHECK_EQUAL(false, result);
}

TEST(EasTime, IsEqualOrAfter0xFFFFFF00and100False)
{
    bool result = eas_time_is_equal_or_after(0xFFFFFF00, 100);
    CHECK_EQUAL(false, result);
}

TEST(EasTime, IsEqualOrAfter7FFFFFFFand0False)
{
    bool result = eas_time_is_equal_or_after(0x7FFFFFFF, 0);
    CHECK_EQUAL(true, result);
}

/* The difference between the two values is exactly 0x80000000, which is the halfway point, i.e.
 * (0x80000000 - 0) == (0 - 0x80000000) == (0x80000000) in unsigned 32-bit arithmetic. At this point, we do not know,
 * and cannot assume, which value was recorded first, so the result is false. */
TEST(EasTime, IsEqualOrAfter80000000and0False)
{
    bool result = eas_time_is_equal_or_after(0x80000000, 0);
    CHECK_EQUAL(false, result);
}

/* Difference between the two values is less in the wraparound direction, so we assume that 0x80000001 was recorded
 * first, and 0 after it. */
TEST(EasTime, IsEqualOrAfter80000001and0False)
{
    bool result = eas_time_is_equal_or_after(0x80000001, 0);
    CHECK_EQUAL(false, result);
}
