#include "CppUTestExt/MockSupport.h"
#include "mock_ops_queue_start_op.h"

void mock_ops_queue_start_op(void *op, void *user_data)
{
    size_t op_size = mock().getData("opSize").getUnsignedIntValue();
    mock()
        .actualCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)op, op_size)
        .withParameter("user_data", user_data);
}
