#include "CppUTestExt/MockSupport.h"
#include "linked_list_node_allocator.h"

static LinkedListNode *mock_linked_list_node_allocator_alloc()
{
    mock().actualCall("linked_list_node_allocator_alloc");
    return (LinkedListNode *)mock().pointerReturnValue();
}

static void mock_linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
    mock().actualCall("linked_list_node_allocator_free").withParameter("linked_list_node", linked_list_node);
}

LinkedListNode *(*linked_list_node_allocator_alloc)() = mock_linked_list_node_allocator_alloc;
void (*linked_list_node_allocator_free)(LinkedListNode *linked_list_node) = mock_linked_list_node_allocator_free;
