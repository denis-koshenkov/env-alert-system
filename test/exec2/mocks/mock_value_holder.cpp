#include <stdbool.h>
#include <string.h>

#include "CppUTestExt/MockSupport.h"
#include "mock_value_holder.h"

struct value_holder_struct {};

value_holder value_holder_create(uint8_t *value_buf, size_t value_size)
{
    mock().actualCall("value_holder_create").withParameter("p2", value_size);
    return NULL;
}
