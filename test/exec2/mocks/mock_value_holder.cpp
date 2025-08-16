#include <stdbool.h>
#include <string.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_value_holder.h"

struct ValueHolderStruct {};

ValueHolder value_holder_create(uint8_t *value_buf, size_t value_size)
{
    mock().actualCall("value_holder_create").withParameter("value_size", value_size);
    return (ValueHolder)mock().pointerReturnValue();
}

void value_holder_set(ValueHolder vh, const void *value)
{
    size_t value_size = mock().getData("value_holder_value_size").getUnsignedIntValue();
    mock()
        .actualCall("value_holder_set")
        .withParameter("vh", vh)
        .withMemoryBufferParameter("value", (const uint8_t *)value, value_size);
}

void value_holder_get(ValueHolder vh, void *value)
{
    mock().actualCall("value_holder_get").withParameter("vh", vh).withOutputParameter("value", value);
}

bool value_holder_is_value_changed(ValueHolder vh)
{
    mock().actualCall("value_holder_is_value_changed").withParameter("vh", vh);
    return mock().boolReturnValue();
}
