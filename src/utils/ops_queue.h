#ifndef ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H
#define ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct OpsQueueStruct *OpsQueue;

typedef void (*OpsQueueStartOp)(void *op, void *user_data);

OpsQueue ops_queue_create(size_t op_size, size_t num_ops, void *ops_buf, void *op_buf, OpsQueueStartOp start_op,
                          void *start_op_user_data);

void ops_queue_add_op(OpsQueue self, void *op);

void ops_queue_op_complete(OpsQueue self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H */
