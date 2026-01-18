#ifndef ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H
#define ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct OpsQueueStruct *OpsQueue;

typedef void (*OpsQueueStartOp)(void *op, void *user_data);

/**
 * @brief
 *
 * @param op_size Size of one operation in bytes.
 * @param num_ops Number of operations that can be in progress at the same time. This is the number of times @p
 * ops_queue_add_op can be called before it fires an assert (given that @p ops_queue_op_complete is not called at all).
 * @param ops_buf Must be of size (@p op_size * (@p num_ops - 1)). -1 because the first operation never needs to be
 * stored anywhere - it can be started immediately.
 * @param op_buf
 * @param start_op
 * @param start_op_user_data
 *
 * @return OpsQueue
 */
OpsQueue ops_queue_create(size_t op_size, size_t num_ops, void *ops_buf, void *op_buf, OpsQueueStartOp start_op,
                          void *start_op_user_data);

void ops_queue_add_op(OpsQueue self, void *op);

void ops_queue_op_complete(OpsQueue self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H */
