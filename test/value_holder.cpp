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
    value_holder_set(vh, (void *)&value_set);
    uint32_t value_get;
    value_holder_get(vh, (void *)&value_get);
    CHECK_EQUAL(value_set, value_get);
}

TEST(ValueHolder, GetWhatWeSetUint8t)
{
    uint8_t value_buf;
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t));
    uint8_t value_set = 0x5A;
    value_holder_set(vh, (void *)&value_set);
    uint8_t value_get;
    value_holder_get(vh, (void *)&value_get);
    CHECK_EQUAL(value_set, value_get);
}

TEST(ValueHolder, GetWhatWeSet8bytes)
{
    uint8_t value_buf[8];
    value_holder vh = value_holder_create((uint8_t *)&value_buf, sizeof(uint8_t) * 8);
    uint8_t value_set[8] = {0xFF, 0x5A, 0x00, 0xA5, 0x0F, 0xF0, 0x01, 0x65};
    value_holder_set(vh, (void *)&value_set);
    uint8_t value_get[8];
    value_holder_get(vh, (void *)&value_get);
    CHECK(memcmp(value_set, value_get, 8) == 0);
}
