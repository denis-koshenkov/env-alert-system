#include <stdint.h>
#include <cstring>

#include "CppUTestExt/MockSupport.h"

#include "config.h"
#include "mock_variable_requirement_allocator.h"
#include "eas_assert.h"

static uint8_t variable_requirement_buffer[CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE];
static bool is_allocated = false;

void *variable_requirement_allocator_alloc()
{
    EAS_ASSERT(!is_allocated);
    memset(variable_requirement_buffer, 0, CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE);
    is_allocated = true;
    return variable_requirement_buffer;
}

void variable_requirement_allocator_free(void *buf)
{
    EAS_ASSERT(is_allocated);
    is_allocated = false;
}
