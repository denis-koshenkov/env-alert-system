#ifndef TEST_INTERNAL_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_H
#define TEST_INTERNAL_FAKE_VARIABLE_REQUIREMENT_ALLOCATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

void *fake_variable_requirement_allocator_alloc();

void fake_variable_requirement_allocator_free(void *buf);

#ifdef __cplusplus
}
#endif

#endif
