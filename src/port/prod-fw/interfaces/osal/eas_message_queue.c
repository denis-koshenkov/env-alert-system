#include <zephyr/kernel.h>

#include "osal/eas_message_queue.h"
#include "config.h"
#include "eas_assert.h"

#ifndef CONFIG_EAS_MESSAGE_QUEUE_MAX_NUM_INSTANCES
#define CONFIG_EAS_MESSAGE_QUEUE_MAX_NUM_INSTANCES 1
#endif

typedef struct EasMessageQueueStruct {
    k_pipe pipe;
    uint8_t max_message_size;
} EasMessageQueueStruct;

static struct EasMessageQueueStruct instances[CONFIG_EAS_MESSAGE_QUEUE_MAX_NUM_INSTANCES];
static size_t instance_idx = 0;

EasMessageQueue eas_message_queue_create(uint8_t *const buf, size_t buf_size, uint8_t max_message_size)
{
    EAS_ASSERT(instance_idx < CONFIG_EAS_MESSAGE_QUEUE_MAX_NUM_INSTANCES);
    struct EasMessageQueueStruct *instance = &instances[instance_idx];
    instance_idx++;

    instance->max_message_size = max_message_size;
    k_pipe_init(&(instance->pipe), buf, buf_size);

    return instance;
}

bool eas_message_queue_push(EasMessageQueue self, const uint8_t *const message, uint8_t message_size)
{
    EAS_ASSERT(self);
    EAS_ASSERT(message);
    EAS_ASSERT(message_size <= self->max_message_size);

    /* Write 1 byte that represents the number of bytes in the message */
    int rc = k_pipe_write(&(self->pipe), &message_size, 1, K_NO_WAIT);
    if (rc != 1) {
        return false;
    }

    /* Write the message bytes */
    rc = k_pipe_write(&(self->pipe), message, message_size, K_NO_WAIT);
    if (rc != message_size) {
        return false;
    }

    return true;
}

uint8_t eas_message_queue_pop(EasMessageQueue self, uint8_t *const message)
{
    EAS_ASSERT(self);
    EAS_ASSERT(message);

    uint8_t message_size = 0;
    int rc = k_pipe_read(&(self->pipe), &message_size, 1, K_FOREVER);
    /* Since we block forever until the byte is available, this should always succeed */
    EAS_ASSERT(rc == 1);
    EAS_ASSERT(message_size <= self->max_message_size);

    rc = k_pipe_read(&(self->pipe), message, message_size, K_FOREVER);
    /* Since we block forever, this should always read out message_size bytes from the pipe */
    EAS_ASSERT(rc == message_size);

    return message_size;
}
