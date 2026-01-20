#ifndef ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H
#define ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

typedef struct OpsQueueStruct *OpsQueue;

/**
 * @brief Start an operation.
 *
 * User-defined callback that must start the @p op operation. Once the operation is complete, @ref ops_queue_op_complete
 * must be called.
 *
 * @param op Operation to start. Points to a buffer of size "op_size" that was passed to @ref ops_queue_create.
 * @param user_data "start_op_user_data" parameter that was passed to @ref ops_queue_create.
 */
typedef void (*OpsQueueStartOp)(void *op, void *user_data);

/**
 * @brief Create an operations queue instance.
 *
 * @param[in] op_size Size of one operation in bytes. Must be > 0.
 * @param[in] num_ops Number of operations that can be in progress at the same time. Must be > 1. This is the number of
 * times @p ops_queue_add_op can be called before it fires an assert (given that @p ops_queue_op_complete is not called
 * at all).
 * @param[in] ops_buf Must be of size ( @p op_size * ( @p num_ops - 1 )). -1 because the first operation never needs to
 * be stored anywhere - it can be started immediately.
 * @param[in] op_buf Buffer of size @p op_size. Needed for internal implementation of the operations queue.
 * @param[in] start_op User-defined callback that starts the operation.
 * @param[in] start_op_user_data User data to pass to @p start_op.
 *
 * @return OpsQueue Created operations queue instance.
 */
OpsQueue ops_queue_create(size_t op_size, size_t num_ops, void *ops_buf, void *op_buf, OpsQueueStartOp start_op,
                          void *start_op_user_data);

/**
 * @brief Add an operation to the operations queue.
 *
 * @param self Operations queue instance returned by @ref ops_queue_create.
 * @param op Operation to add.
 */
void ops_queue_add_op(OpsQueue self, void *op);

/**
 * @brief Notify the operations queue that the in progress operation is complete.
 *
 * If there are any pending operations in the queue, the next operation is started.
 *
 * @param self Operations queue instance returned by @ref ops_queue_create.
 */
void ops_queue_op_complete(OpsQueue self);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_UTILS_OPS_QUEUE_H */
