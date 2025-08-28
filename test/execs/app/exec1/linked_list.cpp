#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "utils/linked_list.h"
#include "fake_linked_list_node_allocator.h"
#include "config.h"

typedef struct LinkedListIdElement {
    uint8_t id;
} LinkedListIdElement;

#define LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS 10
static bool actual_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];
static bool expected_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];

static void expect_id_element_in_list(uint8_t id)
{
    expected_elements_in_list[id] = true;
}

static void actual_id_element_detected_in_list(uint8_t id)
{
    actual_elements_in_list[id] = true;
}

static void reset_expected_actual_id_elements()
{
    memset(expected_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
    memset(actual_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
}

static bool expected_id_elements_match_actual()
{
    return (memcmp(expected_elements_in_list, actual_elements_in_list,
                   LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool)) == 0);
}

static void for_each_cb_id_elements(void *element, void *user_data)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    actual_id_element_detected_in_list(id_element->id);
}

// clang-format off
TEST_GROUP(LinkedList)
{
    void setup()
    {
        reset_expected_actual_id_elements();
    }
};
// clang-format on

TEST(LinkedList, AddRaisesAssertIfListIsNull)
{
    LinkedListIdElement id_element_0 = {.id = 0};

    LinkedList linked_list = linked_list_create();
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_add");
    linked_list_add(NULL, &id_element_0);
}

TEST(LinkedList, ListEmptyAfterCreate)
{
    LinkedList linked_list = linked_list_create();
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, ElementIsTheOnlyInListAfterAdding)
{
    LinkedListIdElement id_element_3 = {.id = 3};
    LinkedListNode *id_element_3_node = fake_linked_list_node_allocator_alloc();
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    expect_id_element_in_list(3);

    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_3);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    CHECK_TRUE(expected_id_elements_match_actual());

    /* Clean up */
    fake_linked_list_node_allocator_free(id_element_3_node);
}

TEST(LinkedList, TwoElementsInListAfterAddingTwoElements)
{
    LinkedList linked_list = linked_list_create();

    LinkedListNode *id_element_0_node = fake_linked_list_node_allocator_alloc();
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    LinkedListNode *id_element_2_node = fake_linked_list_node_allocator_alloc();
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);

    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_2 = {.id = 2};

    expect_id_element_in_list(0);
    expect_id_element_in_list(2);

    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_2);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    CHECK_TRUE(expected_id_elements_match_actual());

    /* Clean up */
    fake_linked_list_node_allocator_free(id_element_0_node);
    fake_linked_list_node_allocator_free(id_element_2_node);
}

TEST(LinkedList, AddFiresAssertIfFailedToAllocateNode)
{
    LinkedList linked_list = linked_list_create();
    LinkedListIdElement element = {.id = 0};

    /* linked_list_node_allocator_alloc returns NULL if it fails to allocate the node. We expect linked_list_add to
     * detect this and fire an assert. */
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue((LinkedListNode *)NULL);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("new_node", "linked_list_add");

    linked_list_add(linked_list, &element);
}
