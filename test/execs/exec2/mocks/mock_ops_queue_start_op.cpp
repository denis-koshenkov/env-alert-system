#include "CppUTestExt/MockSupport.h"
#include "mock_ops_queue_start_op.h"

void mock_ops_queue_start_op(void *op, void *user_data)
{
    mock().actualCall("mock_ops_queue_start_op").withParameter("op", op).withParameter("user_data", user_data);
}
