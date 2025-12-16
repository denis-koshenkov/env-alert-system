#include <stddef.h>

#include <zephyr/sys/mem_blocks.h>

#include "led_notification_allocator.h"
#include "config.h"
#include "utils/util.h"
#include "eas_assert.h"

/** Zephyr requirement: block size must be at least 4N bytes long, where N is greater than 0. */
#define LED_NOTIFICATION_ALLOCATOR_BLOCK_SIZE DIV4_UP(sizeof(LedNotification))

/** The buffer must be aligned to an N-byte boundary, where N is a power of 2 larger than 2 (i.e. 4, 8, 16, ...). The
 * block size should also be a multiple of N. In our case, N is 4. We align the buffer to a 4-byte boundary and ensure
 * that the size of each block is a multiple of 4. 4 is chosen as the lowest allowed number, so that we do not have to
 * waste more space in each block. */
#define LED_NOTIFICATION_ALLOCATOR_BUF_ALIGNMENT 4

SYS_MEM_BLOCKS_DEFINE_STATIC(led_notification_allocator, LED_NOTIFICATION_ALLOCATOR_BLOCK_SIZE,
                             CONFIG_LED_NOTIFICATION_ALLOCATOR_NUM_NOTIFICATIONS,
                             LED_NOTIFICATION_ALLOCATOR_BUF_ALIGNMENT);

LedNotification *led_notification_allocator_alloc()
{
    void *out_block = NULL;
    int ret = sys_mem_blocks_alloc(&led_notification_allocator, 1, &out_block);
    return (ret == 0) ? ((LedNotification *)out_block) : NULL;
}

void led_notification_allocator_free(LedNotification *led_notification)
{
    int ret = sys_mem_blocks_free(&led_notification_allocator, 1, (void **)&led_notification);
    EAS_ASSERT(ret == 0);
}
