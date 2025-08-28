#include "CppUTestExt/MockSupport.h"
#include "mock_linked_list_node_allocator.h"

LinkedListNode *linked_list_node_allocator_alloc()
{
    mock().actualCall("linked_list_node_allocator_alloc");
    return (LinkedListNode *)mock().pointerReturnValue();
}

void linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
    mock().actualCall("linked_list_node_allocator_free").withParameter("linked_list_node", linked_list_node);
}
