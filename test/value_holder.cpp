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
    TestAssertPlugin::expectAssertion("t");
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
    TestAssertPlugin::expectAssertion("t");
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
    TestAssertPlugin::expectAssertion("t->set_has_been_called");
    value_holder_get(vh, &value);
}
