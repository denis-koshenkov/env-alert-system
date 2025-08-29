#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "utils/linked_list.h"
#include "fake_linked_list_node_allocator.h"
#include "config.h"

typedef struct LinkedListIdElement {
    uint8_t id;
    bool condition_evaluation_result;
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

static LinkedListNode *id_element_0_node;
static LinkedListNode *id_element_1_node;
static LinkedListNode *id_element_2_node;
static LinkedListNode *id_element_3_node;
static LinkedListNode *id_element_4_node;

// clang-format off
TEST_GROUP(LinkedList)
{
    void setup()
    {
        reset_expected_actual_id_elements();

        id_element_0_node = fake_linked_list_node_allocator_alloc();
        id_element_1_node = fake_linked_list_node_allocator_alloc();
        id_element_2_node = fake_linked_list_node_allocator_alloc();
        id_element_3_node = fake_linked_list_node_allocator_alloc();
        id_element_4_node = fake_linked_list_node_allocator_alloc();
    }

    void teardown()
    {
        fake_linked_list_node_allocator_free(id_element_0_node);
        fake_linked_list_node_allocator_free(id_element_1_node);
        fake_linked_list_node_allocator_free(id_element_2_node);
        fake_linked_list_node_allocator_free(id_element_3_node);
        fake_linked_list_node_allocator_free(id_element_4_node);
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

TEST(LinkedList, ForEachRaisesAssertIfListIsNull)
{
    LinkedList linked_list = linked_list_create();
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_for_each");
    linked_list_for_each(NULL, for_each_cb_id_elements, NULL);
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

TEST(LinkedList, ListEmptyAfterCreate)
{
    LinkedList linked_list = linked_list_create();
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, ElementIsTheOnlyInListAfterAdding)
{
    LinkedListIdElement id_element_3 = {.id = 3};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    expect_id_element_in_list(3);

    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_3);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, TwoElementsInListAfterAddingTwoElements)
{

    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_2 = {.id = 2};
    expect_id_element_in_list(0);
    expect_id_element_in_list(2);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);

    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_2);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    CHECK_TRUE(expected_id_elements_match_actual());
}

static void *actual_user_data = 0;

static void save_actual_user_data_cb(void *element, void *user_data)
{
    actual_user_data = user_data;
}

TEST(LinkedList, ForEachPassesUserDataToCallback)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);

    LinkedList linked_list = linked_list_create();
    uint8_t expected_user_data = 42;
    linked_list_add(linked_list, &id_element_0);
    linked_list_for_each(linked_list, save_actual_user_data_cb, &expected_user_data);

    CHECK_EQUAL(&expected_user_data, actual_user_data);
}

static bool condition_true_cb(void *element)
{
    return true;
}

static bool condition_false_cb(void *element)
{
    return false;
}

static bool condition_id_element_cb(void *element)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    return id_element->condition_evaluation_result;
}

TEST(LinkedList, RemoveOnConditionEmptyListStillEmpty)
{
    LinkedList linked_list = linked_list_create();
    linked_list_remove_on_condition(linked_list, condition_true_cb);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, RemoveOnConditionDoesNotRemoveElementConditionFalse)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    /* Since condition evaluates to false, we expect remove_on_condition to keep it in the list. */
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, condition_false_cb);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, RemoveOnConditionRemovesElementConditionTrue)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    /* We expect the list to be empty, so we are not calling expect_id_element_in_list here. */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, condition_true_cb);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, RemoveOnConditionRemovesAllElementsConditionTrue)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_1 = {.id = 1};
    LinkedListIdElement id_element_2 = {.id = 2};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_2_node);
    /* We expect the list to be empty, so we are not calling expect_id_element_in_list here. */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_1);
    linked_list_add(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_true_cb);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, RemoveOnConditionKeepsAllElementsConditionFalse)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_1 = {.id = 1};
    LinkedListIdElement id_element_2 = {.id = 2};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    /* We expect all three elements to be in the list, since condition is false for all of them. */
    expect_id_element_in_list(0);
    expect_id_element_in_list(1);
    expect_id_element_in_list(2);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_1);
    linked_list_add(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_false_cb);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}

TEST(LinkedList, RemoveOnConditionRemovesElementsWithConditionTrueKeepsElementsWithConditionFalse)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = true};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = true};
    LinkedListIdElement id_element_3 = {.id = 3, .condition_evaluation_result = false};
    LinkedListIdElement id_element_4 = {.id = 4, .condition_evaluation_result = true};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_4_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_4_node);
    /* Condition is false for elements 0, 3 - they should be kept, condition true false for elements 1, 2, 4 - they
     * should be removed. */
    expect_id_element_in_list(0);
    expect_id_element_in_list(3);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_1);
    linked_list_add(linked_list, &id_element_2);
    linked_list_add(linked_list, &id_element_3);
    linked_list_add(linked_list, &id_element_4);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    CHECK_TRUE(expected_id_elements_match_actual());
}
