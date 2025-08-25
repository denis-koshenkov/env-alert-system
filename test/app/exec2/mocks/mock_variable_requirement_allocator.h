#ifndef MOCK_VARIABLE_REQUIREMENT_ALLOCATOR_H
#define MOCK_VARIABLE_REQUIREMENT_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

void *variable_requirement_allocator_alloc();

void variable_requirement_allocator_free(void *buf);

#ifdef __cplusplus
}
#endif

#endif
