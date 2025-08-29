#include <stdint.h>

#include "fake_variable_requirement_allocator.h"
#include "memory_block_allocator.h"
#include "config.h"

#ifndef CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS
#define CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS 1
#endif

static uint8_t requirements_buffer[CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE *
                                   CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS];
static bool free_requirements_map[CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS];

static MemoryBlockAllocator get_block_allocator_instance()
{
    static MemoryBlockAllocator instance;
    static bool is_created = false;
    if (!is_created) {
        instance = memory_block_allocator_create(CONFIG_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_NUM_REQUIREMENTS,
                                                 CONFIG_VARIABLE_REQUIREMENT_MAX_SIZE, requirements_buffer,
                                                 free_requirements_map);
        is_created = true;
    }
    return instance;
}

void *fake_variable_requirement_allocator_alloc()
{
    return memory_block_allocator_alloc(get_block_allocator_instance());
}

void fake_variable_requirement_allocator_free(void *buf)
{
    memory_block_allocator_free(get_block_allocator_instance(), buf);
}
