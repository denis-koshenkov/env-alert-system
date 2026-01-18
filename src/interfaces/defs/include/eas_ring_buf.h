#ifndef ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_RING_BUF_H
#define ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_RING_BUF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>

typedef struct EasRingBufStruct *EasRingBuf;

/**
 * @brief Create a ring buffer instance.
 *
 * @param[in] elem_size Size of one element.
 * @param[in] num_elems Number of elements that can fit into the ring buffer at the same time.
 * @param[in] buffer Buffer to hold the elements, must be of size ( @p elem_size * @p num_elems ).
 *
 * @return EasRingBuf Created ring buffer instance.
 */
EasRingBuf eas_ring_buf_create(size_t elem_size, size_t num_elems, void *const buffer);

/**
 * @brief Push an element to the ring buffer.
 *
 * @param[in] self Ring buffer instance returned by @ref eas_ring_buf_create.
 * @param[in] element Element to push to the buffer. Must be of size elem_size that was passed to @ref
 * eas_ring_buf_create.
 *
 * @return true Successfully pushed the element.
 * @return false Failed to push the element because buffer is full.
 */
bool eas_ring_buf_push(EasRingBuf self, const void *const element);

/**
 * @brief Pop an element from the ring buffer.
 *
 * @param[in] self Ring buffer instance returned by @ref eas_ring_buf_create.
 * @param[out] element The popped element gets written to this buffer. Must be of size elem_size that was passed to @ref
 * eas_ring_buf_create.
 *
 * @return true Successfully popped the element.
 * @return false Failed to pop the element because the buffer is empty.
 */
bool eas_ring_buf_pop(EasRingBuf self, void *const element);

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_INTERFACES_DEFS_INCLUDE_EAS_RING_BUF_H */
