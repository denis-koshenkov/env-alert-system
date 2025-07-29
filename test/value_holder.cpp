#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

extern "C"
{
#include "value_holder.h"
}

TEST_GROUP(ValueHolder){};

TEST(ValueHolder, GetWhatWeSetUint32t)
{
    uint32_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint32_t));
    uint32_t value_set = 1;
    value_holder_set(vh, &value_set);
    uint32_t value_get;
    value_holder_get(vh, &value_get);
    CHECK_EQUAL(value_set, value_get);
}

TEST(ValueHolder, GetWhatWeSetUint8t)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value_set = 0x5A;
    value_holder_set(vh, &value_set);
    uint8_t value_get;
    value_holder_get(vh, &value_get);
    CHECK_EQUAL(value_set, value_get);
}

TEST(ValueHolder, GetWhatWeSet8bytes)
{
    uint8_t value_buf[8];
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t) * 8);
    uint8_t value_set[8] = {0xFF, 0x5A, 0x00, 0xA5, 0x0F, 0xF0, 0x01, 0x65};
    value_holder_set(vh, &value_set);
    uint8_t value_get[8];
    value_holder_get(vh, &value_get);
    CHECK(memcmp(value_set, value_get, 8) == 0);
}

TEST(ValueHolder, SetRaisesAssertVhNullPointer)
{
    uint8_t dummy_buf;
    TestAssertPlugin::expectAssertion("vh");
    value_holder_set(NULL, &dummy_buf);
}

TEST(ValueHolder, SetRaisesAssertValueNullPointer)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create(&value_buf, sizeof(uint8_t));
    TestAssertPlugin::expectAssertion("value");
    value_holder_set(vh, NULL);
}

TEST(ValueHolder, GetRaisesAssertVhNullPointer)
{
    uint8_t dummy_buf;
    TestAssertPlugin::expectAssertion("vh");
    value_holder_get(NULL, &dummy_buf);
}

TEST(ValueHolder, GetRaisesAssertValueNullPointer)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create(&value_buf, sizeof(uint8_t));
    TestAssertPlugin::expectAssertion("value");
    value_holder_get(vh, NULL);
}

TEST(ValueHolder, GetRaisesAssertIfCalledBeforeSet)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create(&value_buf, sizeof(uint8_t));
    uint8_t value;
    TestAssertPlugin::expectAssertion("vh->set_has_been_called");
    value_holder_get(vh, &value);
}

TEST(ValueHolder, IsValueChangedReturnsTrueAfterSetIsCalledOnce)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value = 255;
    value_holder_set(vh, &value);
    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(is_value_changed);
}

TEST(ValueHolder, IsValueChangedReturnsTrueAfterSetIsCalledOnceAnotherValue)
{
    uint32_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint32_t));
    uint32_t value = 0x5A5AA5A5;
    value_holder_set(vh, &value);
    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(is_value_changed);
}

TEST(ValueHolder, IsValueChangedReturnsFalseAfterSameValuesAreSet)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value = 125;
    value_holder_set(vh, &value);
    value_holder_set(vh, &value);
    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(!is_value_changed);
}

TEST(ValueHolder, IsValueChangedReturnsTrueAfterDifferentValuesAreSet)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value1 = 199;
    value_holder_set(vh, &value1);
    uint8_t value2 = 200;
    value_holder_set(vh, &value2);
    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(is_value_changed);
}

TEST(ValueHolder, IsValueChangedRaisesAssertIfCalledBeforeSet)
{
    uint32_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint32_t));
    TestAssertPlugin::expectAssertion("vh->set_has_been_called");
    value_holder_is_value_changed(vh);
}

TEST(ValueHolder, IsValueChangedRaisesAssertVhNullPointerAfterSetCalled)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value = 0;
    value_holder_set(vh, &value);
    TestAssertPlugin::expectAssertion("vh");
    value_holder_is_value_changed(NULL);
}

TEST(ValueHolder, IsValueChangedReturnsTrueAfterManyDifferentValuesAreSet)
{
    uint16_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint16_t));

    /* The last two values are different, so we expect is_value_changed == true */
    uint16_t values[] = {2020, 1010, 0xFFFF, 65, 33333, 44, 1};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        value_holder_set(vh, &values[i]);
    }

    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(is_value_changed);
}

TEST(ValueHolder, IsValueChangedReturnsTrueAfterTwoLastValuesAreEqual)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));

    /* The last two values are equal, so we expect is_value_changed == false */
    uint8_t values[] = {11, 111, 222, 0x5A, 255, 32, 23, 23};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        value_holder_set(vh, &values[i]);
    }

    bool is_value_changed = value_holder_is_value_changed(vh);
    CHECK(!is_value_changed);
}
