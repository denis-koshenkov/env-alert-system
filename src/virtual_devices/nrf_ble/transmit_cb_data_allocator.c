#include <stddef.h>

#include <zephyr/sys/mem_blocks.h>

#include "transmit_cb_data_allocator.h"
#include "util.h"
#include "config.h"
#include "eas_assert.h"

/** Zephyr requirement: block size must be at least 4N bytes long, where N is greater than 0. */
#define TRANSMIT_CB_DATA_ALLOCATOR_BLOCK_SIZE DIV4_UP(sizeof(NrfBleTransceiverTransmitCbData))

/** The buffer must be aligned to an N-byte boundary, where N is a power of 2 larger than 2 (i.e. 4, 8, 16, ...). The
 * block size should also be a multiple of N. In our case, N is 4. We align the buffer to a 4-byte boundary and ensure
 * that the size of each block is a multiple of 4. 4 is chosen as the lowest allowed number, so that we do not have to
 * waste more space in each block. */
#define TRANSMIT_CB_DATA_ALLOCATOR_BUF_ALIGNMENT 4

SYS_MEM_BLOCKS_DEFINE_STATIC(transmit_cb_data_allocator, TRANSMIT_CB_DATA_ALLOCATOR_BLOCK_SIZE,
                             CONFIG_TRANSMIT_CB_DATA_ALLOCATOR_NUM_BLOCKS, TRANSMIT_CB_DATA_ALLOCATOR_BUF_ALIGNMENT);

NrfBleTransceiverTransmitCbData *transmit_cb_data_allocator_alloc()
{
    void *out_block = NULL;
    int ret = sys_mem_blocks_alloc(&transmit_cb_data_allocator, 1, &out_block);
    return (ret == 0) ? ((NrfBleTransceiverTransmitCbData *)out_block) : NULL;
}

void transmit_cb_data_allocator_free(NrfBleTransceiverTransmitCbData *transmit_cb_data)
{
    int ret = sys_mem_blocks_free(&transmit_cb_data_allocator, 1, (void **)&transmit_cb_data);
    EAS_ASSERT(ret == 0);
}
