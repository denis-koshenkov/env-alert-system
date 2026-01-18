#include <stddef.h>

#include "ring_buf_get_inst_buf.h"
#include "ring_buf_private.h"
#include "config.h"

#ifndef CONFIG_RING_BUFFER_MAX_NUM_INSTANCES
#define CONFIG_RING_BUFFER_MAX_NUM_INSTANCES 1
#endif

void *ring_buf_get_inst_buf(void *user_data)
{
    static struct RingBufStruct instances[CONFIG_RING_BUFFER_MAX_NUM_INSTANCES];
    static size_t idx = 0;
    return (idx < CONFIG_RING_BUFFER_MAX_NUM_INSTANCES) ? (&(instances[idx++])) : NULL;
}
