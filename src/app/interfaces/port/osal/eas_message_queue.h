#ifndef ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_OSAL_EAS_MESSAGE_QUEUE_H
#define ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_OSAL_EAS_MESSAGE_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Multiple producer, single consumer message queue for messages of variable sizes.
 */

typedef struct EasMessageQueueStruct *EasMessageQueue;

/**
 * @brief Create a message queue instance.
 *
 * @param buf Memory buffer to use for storing the messages in the queue.
 * @param buf_size Length of the provided @p buf.
 * @param max_message_size Maximum size of one message. Cannot be greater than 255.
 *
 * @return EasMessageQueue Created message queue instance.
 */
EasMessageQueue eas_message_queue_create(uint8_t *const buf, size_t buf_size, uint8_t max_message_size);

/**
 * @brief Push a message to the message queue.
 *
 * This function does not block. If there is not enough space in the queue to push this message, returns false
 * immediately instead of blocking.
 *
 * This function is thread-safe - it is allowed to push to the same queue from different contexts - both threads and
 * ISRs.
 *
 * @param message Message payload.
 * @param message_size Number of bytes in the @p message array. Cannot be more than max_message_size passed to @ref
 * message_queue_create.
 *
 * @return true Message was successfully pushed to the queue.
 * @return false Failed to push message, because there is not enough space in the buffer.
 *
 * @note Fires an assert if @p message_size > max_message_size passed to @ref eas_message_queue_create.
 */
bool eas_message_queue_push(EasMessageQueue self, const uint8_t *const message, uint8_t message_size);

/**
 * @brief Pop a message from the message queue.
 *
 * This is a blocking function. The calling thread is blocked until a message becomes available in the message queue.
 *
 * @param self Message queue instance returned by @ref message_queue_create.
 * @param[out] message Received message is written to this buffer. The buffer should be of size max_message_size passed
 * to @ref message_queue_create, so that the largest allowed message can fully fit into the buffer.
 *
 * @return size_t Number of bytes in the popped message.
 *
 * @warning Do not call from ISRs - ISRs are not allowed to block.
 */
uint8_t eas_message_queue_pop(EasMessageQueue self, uint8_t *const message);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_PORT_INTERFACES_OSAL_EAS_MESSAGE_QUEUE_H */
