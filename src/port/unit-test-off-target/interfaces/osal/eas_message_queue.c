#include "osal/eas_message_queue.h"

/* No implementation - when the application runs on unit test port, message queues are not used. */

EasMessageQueue eas_message_queue_create(uint8_t *const buf, size_t buf_size, uint8_t max_message_size)
{
    return NULL;
}

bool eas_message_queue_push(EasMessageQueue self, const uint8_t *const message, uint8_t message_size)
{
    return false;
}

uint8_t eas_message_queue_pop(EasMessageQueue self, uint8_t *const message)
{
    return 0;
}
