#include <stddef.h>

#include "ops_queue.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES
#define CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES 1
#endif

struct OpsQueueStruct {
    OpsQueueStartOp start_op;
    void *start_op_user_data;
};

static struct OpsQueueStruct instances[CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

OpsQueue ops_queue_create(size_t op_size, size_t num_ops, void *ops_buf, OpsQueueStartOp start_op, void *user_data)
{
    EAS_ASSERT(instance_idx < CONFIG_OPS_QUEUE_MAX_NUM_INSTANCES);
    struct OpsQueueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->start_op = start_op;
    instance->start_op_user_data = user_data;

    return instance;
}

void ops_queue_add_op(OpsQueue self, void *op)
{
    self->start_op(op, self->start_op_user_data);
}

void ops_queue_op_complete(OpsQueue self)
{
}
