#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "utils/linked_list.h"
#include "utils/linked_list_private.h"
/* TODO: move memory_block_allocator from port to test directory - it is only used in tests. In test, rename subdir from
 * "port" to "internal". */
#include "memory_block_allocator.h"
#include "config.h"

#ifndef CONFIG_MOCK_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES
#define CONFIG_MOCK_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES 1
#endif

static uint8_t nodes_buffer[sizeof(LinkedListNode) * CONFIG_MOCK_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES];
static bool free_nodes_map[CONFIG_MOCK_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES];

static MemoryBlockAllocator get_block_allocator_instance()
{
    static MemoryBlockAllocator instance;
    static bool is_created = false;
    if (!is_created) {
        instance = memory_block_allocator_create(CONFIG_MOCK_LINKED_LIST_NODE_ALLOCATOR_NUM_NODES,
                                                 sizeof(LinkedListNode), nodes_buffer, free_nodes_map);
        is_created = true;
    }
    return instance;
}

static LinkedListNode *mock_linked_list_node_allocator_alloc()
{
    return (LinkedListNode *)memory_block_allocator_alloc(get_block_allocator_instance());
}

static void mock_linked_list_node_allocator_free(LinkedListNode *linked_list_node)
{
    memory_block_allocator_free(get_block_allocator_instance(), (void *)linked_list_node);
}

typedef struct LinkedListIdElement {
    uint8_t id;
} LinkedListIdElement;

typedef struct LinkedListTestElement {
    uint8_t count;
} LinkedListTestElement;

static LinkedListTestElement test_element;

static void for_each_cb_increment_count(void *element, void *user_data)
{
    LinkedListTestElement *iter_element = (LinkedListTestElement *)element;
    iter_element->count++;
}

TEST_GROUP(LinkedList){};

TEST(LinkedList, ListEmptyAfterCreate)
{
    LinkedList linked_list = linked_list_create();

    test_element.count = 0;
    linked_list_for_each(linked_list, for_each_cb_increment_count, NULL);

    CHECK_EQUAL(0, test_element.count);
}

TEST(LinkedList, ElementIsTheOnlyInListAfterAdding)
{
    LinkedList linked_list = linked_list_create();

    LinkedListNode *test_element_node = mock_linked_list_node_allocator_alloc();
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(test_element_node);

    linked_list_add(linked_list, &test_element);

    test_element.count = 0;
    linked_list_for_each(linked_list, for_each_cb_increment_count, NULL);

    CHECK_EQUAL(1, test_element.count);

    /* Clean up */
    mock_linked_list_node_allocator_free(test_element_node);
}

#define LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS 10
static bool actual_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];
static bool expected_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];

static void for_each_cb_id_elements(void *element, void *user_data)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    actual_elements_in_list[id_element->id] = true;
}

// TEST(LinkedList, TwoElementsInListAfterAddingTwoElements)
// {
//     LinkedList linked_list = linked_list_create();

//     LinkedListIdElement id_element_0 = {.id = 0};
//     LinkedListIdElement id_element_2 = {.id = 2};

//     linked_list_add(linked_list, &id_element_0);
//     linked_list_add(linked_list, &id_element_2);

//     memset(expected_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
//     expected_elements_in_list[0] = true;
//     expected_elements_in_list[2] = true;

//     memset(actual_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
//     linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

//     CHECK_TRUE(memcmp(expected_elements_in_list, actual_elements_in_list,
//                       LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool)) == 0);
// }
