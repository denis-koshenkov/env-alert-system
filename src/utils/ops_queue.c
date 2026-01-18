#include <stddef.h>

#include "ops_queue.h"
#include "eas_assert.h"
#include "config.h"
#include "eas_ring_buf.h"

#ifndef CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES
#define CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES 1
#endif

struct OpsQueueStruct {
    OpsQueueStartOp start_op;
    void *start_op_user_data;
    EasRingBuf ops_ring_buf;
    void *op_buf;
    bool op_in_progress;
};

static struct OpsQueueStruct instances[CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

OpsQueue ops_queue_create(size_t op_size, size_t num_ops, void *ops_buf, void *op_buf, OpsQueueStartOp start_op,
                          void *user_data)
{
    EAS_ASSERT(instance_idx < CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES);
    struct OpsQueueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->start_op = start_op;
    instance->start_op_user_data = user_data;
    instance->ops_ring_buf = eas_ring_buf_create(op_size, num_ops, ops_buf);
    instance->op_buf = op_buf;
    instance->op_in_progress = false;

    return instance;
}

void ops_queue_add_op(OpsQueue self, void *op)
{
    if (!self->op_in_progress) {
        self->start_op(op, self->start_op_user_data);
        self->op_in_progress = true;
    } else {
        eas_ring_buf_push(self->ops_ring_buf, op);
    }
}

void ops_queue_op_complete(OpsQueue self)
{
    eas_ring_buf_pop(self->ops_ring_buf, self->op_buf);
    self->start_op(self->op_buf, self->start_op_user_data);
}
