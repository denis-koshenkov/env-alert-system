#include <stddef.h>

#include "eas_ring_buf.h"
#include "config.h"
#include "eas_assert.h"
#include "ring_buf.h"
#include "ring_buf_get_inst_buf.h"

#ifndef CONFIG_EAS_RING_BUF_MAX_NUM_INSTANCES
#define CONFIG_EAS_RING_BUF_MAX_NUM_INSTANCES 1
#endif

struct EasRingBufStruct {
    RingBuf ring_buf;
};

static struct EasRingBufStruct instances[CONFIG_EAS_RING_BUF_MAX_NUM_INSTANCES];
static size_t inst_idx = 0;

EasRingBuf eas_ring_buf_create(size_t elem_size, size_t num_elems, void *const buffer)
{
    EAS_ASSERT(inst_idx < CONFIG_EAS_RING_BUF_MAX_NUM_INSTANCES);
    struct EasRingBufStruct *inst = &instances[inst_idx];
    inst_idx++;

    RingBufInitCfg init_cfg = {
        .get_inst_buf = ring_buf_get_inst_buf,
        .get_inst_buf_user_data = NULL,
        .elem_size = elem_size,
        .num_elems = num_elems,
        .buffer = buffer,
    };
    uint8_t rc = ring_buf_create(&inst->ring_buf, &init_cfg);
    EAS_ASSERT(rc == RING_BUF_RESULT_CODE_OK);
    EAS_ASSERT(inst->ring_buf);

    return inst;
}

bool eas_ring_buf_push(EasRingBuf self, const void *const element)
{
    uint8_t rc = ring_buf_push(self->ring_buf, element);

    bool ret_val = false;
    if (rc == RING_BUF_RESULT_CODE_OK) {
        ret_val = true;
    } else if (rc == RING_BUF_RESULT_CODE_NO_DATA) {
        ret_val = false;
    } else {
        /* Should never get any other return codes */
        EAS_ASSERT(0);
    }
    return ret_val;
}

bool eas_ring_buf_pop(EasRingBuf self, void *const element)
{
    uint8_t rc = ring_buf_pop(self->ring_buf, element);

    bool ret_val = false;
    if (rc == RING_BUF_RESULT_CODE_OK) {
        ret_val = true;
    } else if (rc == RING_BUF_RESULT_CODE_NO_DATA) {
        ret_val = false;
    } else {
        /* Should never get any other return codes */
        EAS_ASSERT(0);
    }
    return ret_val;
}
