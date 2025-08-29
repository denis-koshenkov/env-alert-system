#include "CppUTestExt/MockSupport.h"
#include "mock_variable_requirement_allocator.h"

void *variable_requirement_allocator_alloc()
{
    mock().actualCall("variable_requirement_allocator_alloc");
    return mock().pointerReturnValue();
}

void variable_requirement_allocator_free(void *buf)
{
    mock().actualCall("variable_requirement_allocator_free").withParameter("buf", buf);
}
