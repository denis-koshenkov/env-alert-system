#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "ops_queue.h"
#include "mocks/mock_ops_queue_start_op.h"

struct OpsQueueTestOp {
    uint16_t some_value;
};

#define OPS_QUEUE_TEST_NUM_OPS 2
OpsQueueTestOp ops_buf[OPS_QUEUE_TEST_NUM_OPS];

TEST_GROUP(OpsQueue){};

TEST(OpsQueue, FirstAddCallsStartOp)
{
    OpsQueueTestOp op = {.some_value = 2};
    void *user_data = (void *)0x1;

    OpsQueue inst =
        ops_queue_create(sizeof(OpsQueueTestOp), OPS_QUEUE_TEST_NUM_OPS, ops_buf, mock_ops_queue_start_op, user_data);
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withParameter("op", (void *)&op)
        .withParameter("user_data", user_data);
    ops_queue_add_op(inst, &op);
}

TEST(OpsQueue, TwoOps)
{
    OpsQueueTestOp op1 = {.some_value = 2};
    OpsQueueTestOp op2 = {.some_value = 3};
    void *user_data = (void *)0x2;

    OpsQueue inst =
        ops_queue_create(sizeof(OpsQueueTestOp), OPS_QUEUE_TEST_NUM_OPS, ops_buf, mock_ops_queue_start_op, user_data);

    /* ops_queue_add_op for op1 */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withParameter("op", (void *)&op1)
        .withParameter("user_data", user_data);
    ops_queue_add_op(inst, &op1);

    ops_queue_add_op(inst, &op2);

    /* Must be called from inside ops_queue_op_complete for op1 */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withParameter("op", (void *)&op2)
        .withParameter("user_data", user_data);
    ops_queue_op_complete(inst);
}
