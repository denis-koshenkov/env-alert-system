#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "ops_queue.h"
#include "mocks/mock_ops_queue_start_op.h"

struct OpsQueueTestOp {
    uint16_t some_value;
};

#define OPS_QUEUE_TEST_NUM_OPS 3
#define OPS_QUEUE_TEST_OPS_BUF_SIZE (OPS_QUEUE_TEST_NUM_OPS - 1)
static OpsQueueTestOp ops_buf[OPS_QUEUE_TEST_OPS_BUF_SIZE];
static OpsQueueTestOp op_buf;

static OpsQueue inst = NULL;
void *start_op_user_data = (void *)0x1;

// clang-format off
TEST_GROUP(OpsQueue){
    void setup()
    {
        /* Pass op size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()` */
        mock().setData("opSize", (unsigned int)sizeof(OpsQueueTestOp));

        /* Reset all data */
        memset(ops_buf, 0, sizeof(OpsQueueTestOp) * OPS_QUEUE_TEST_OPS_BUF_SIZE);
        memset(&op_buf, 0, sizeof(OpsQueueTestOp));
        inst = NULL;

        inst = ops_queue_create(sizeof(OpsQueueTestOp), OPS_QUEUE_TEST_NUM_OPS, ops_buf, &op_buf,
                                     mock_ops_queue_start_op, start_op_user_data);
    }
};
// clang-format on

TEST(OpsQueue, FirstAddCallsStartOp)
{
    OpsQueueTestOp op = {.some_value = 2};

    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op);
}

TEST(OpsQueue, TwoOps)
{
    OpsQueueTestOp op1 = {.some_value = 2};
    OpsQueueTestOp op2 = {.some_value = 3};

    /* ops_queue_add_op for op1 */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op1, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op1);

    ops_queue_add_op(inst, &op2);

    /* Must be called from inside ops_queue_op_complete for op1 */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op2, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);
}

TEST(OpsQueue, OneOpCompletes)
{
    OpsQueueTestOp op = {.some_value = 4};

    /* ops_queue_add_op */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op);

    ops_queue_op_complete(inst);
}

TEST(OpsQueue, AddOpAfterAllComplete)
{
    OpsQueueTestOp op1 = {.some_value = 5};
    OpsQueueTestOp op2 = {.some_value = 6};
    OpsQueueTestOp op3 = {.some_value = 7};

    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op1, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op1);
    ops_queue_op_complete(inst);

    /* op1 is complete, so there are no pending ops. Adding op2 must start it immediately. */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op2, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op2);
}

TEST(OpsQueue, AddOpAssertsWhenQueueFull)
{
    OpsQueueTestOp op1 = {.some_value = 8};
    OpsQueueTestOp op2 = {.some_value = 9};
    OpsQueueTestOp op3 = {.some_value = 10};
    OpsQueueTestOp op4 = {.some_value = 11};

    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op1, sizeof(OpsQueueTestOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op1);

    ops_queue_add_op(inst, &op2);
    ops_queue_add_op(inst, &op3);

    /* At this point, the queue is full. Adding another op must trigger an assert. */
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("pushed", "ops_queue_add_op");
    ops_queue_add_op(inst, &op4);
}
