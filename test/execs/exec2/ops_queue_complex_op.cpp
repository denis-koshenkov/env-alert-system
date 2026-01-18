#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "ops_queue.h"
#include "mocks/mock_ops_queue_start_op.h"

struct OpsQueueComplexOp {
    uint32_t u32;
    uint8_t u8;
};

#define OPS_QUEUE_TEST_COMPLEX_OP_NUM_OPS 4
#define OPS_QUEUE_TEST_COMPLEX_OP_OPS_BUF_SIZE (OPS_QUEUE_TEST_COMPLEX_OP_NUM_OPS - 1)
static OpsQueueComplexOp ops_buf[OPS_QUEUE_TEST_COMPLEX_OP_OPS_BUF_SIZE];
static OpsQueueComplexOp op_buf;

static OpsQueue inst = NULL;
void *start_op_user_data = (void *)0x2;

// clang-format off
TEST_GROUP(OpsQueueComplexOp){
    void setup()
    {
        /* Pass op size to mock object, so that it can pass it to the `size` parameter of `withMemoryBufferParameter()` */
        mock().setData("opSize", (unsigned int)sizeof(OpsQueueComplexOp));

        /* Reset all data */
        memset(ops_buf, 0, sizeof(OpsQueueComplexOp) * OPS_QUEUE_TEST_COMPLEX_OP_OPS_BUF_SIZE);
        memset(&op_buf, 0, sizeof(OpsQueueComplexOp));
        inst = NULL;
    }
};
// clang-format on

TEST(OpsQueueComplexOp, StressTest)
{
    inst = ops_queue_create(sizeof(OpsQueueComplexOp), OPS_QUEUE_TEST_COMPLEX_OP_NUM_OPS, ops_buf, &op_buf,
                            mock_ops_queue_start_op, start_op_user_data);

    OpsQueueComplexOp op1 = {.u32 = 0x34728347, .u8 = 0xFF};
    OpsQueueComplexOp op2 = {.u32 = 0xAB458395, .u8 = 0xFE};
    OpsQueueComplexOp op3 = {.u32 = 0x9BBB4323, .u8 = 0xFD};
    OpsQueueComplexOp op4 = {.u32 = 0x12345DDC, .u8 = 0xFC};
    OpsQueueComplexOp op5 = {.u32 = 0x987F5A5A, .u8 = 0xFB};
    OpsQueueComplexOp op6 = {.u32 = 0x01010101, .u8 = 0xFA};
    OpsQueueComplexOp op7 = {.u32 = 0xAAAAAAAA, .u8 = 0xF9};
    OpsQueueComplexOp op8 = {.u32 = 0x65A78D01, .u8 = 0xF8};

    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op1, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op1);
    ops_queue_op_complete(inst);

    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op2, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op2);
    ops_queue_add_op(inst, &op3);
    ops_queue_add_op(inst, &op4);
    ops_queue_add_op(inst, &op5);

    /* Queue is full */

    /* Op 2 is complete, op 3 is started */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op3, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);

    /* Add another op */
    ops_queue_add_op(inst, &op6);

    /* Queue is full again - ops 3 is in progress, 4, 5, and 6 are pending */

    /* Op 3 is complete, op 4 is started */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op4, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);

    /* Op 4 is complete, op 5 is started */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op5, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);

    /* Op 5 is complete, op 6 is started */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op6, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);

    /* Add another op */
    ops_queue_add_op(inst, &op7);

    /* Op 6 is complete, op 7 is started */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op7, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_op_complete(inst);

    /* Op 7 is complete */
    ops_queue_op_complete(inst);

    /* Queue is now empty */

    /* Add another op - must be started immediately since there are no ops in progress */
    mock()
        .expectOneCall("mock_ops_queue_start_op")
        .withMemoryBufferParameter("op", (const uint8_t *)&op8, sizeof(OpsQueueComplexOp))
        .withParameter("user_data", start_op_user_data);
    ops_queue_add_op(inst, &op8);

    /* Op 8 is complete */
    ops_queue_op_complete(inst);

    /* Queue is now empty */
}
