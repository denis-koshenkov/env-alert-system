#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"
#include "CppUTestExt/MockSupport.h"

#include "utils/linked_list.h"
#include "fake_linked_list_node_allocator.h"
#include "config.h"
#include "eas_assert.h"

typedef struct LinkedListIdElement {
    uint8_t id;
    bool condition_evaluation_result;
} LinkedListIdElement;

#define LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS 10
static uint8_t actual_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];
static uint8_t expected_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];
static size_t num_actual_elements_in_list = 0;
static size_t num_expected_elements_in_list = 0;

/** This function should be called in the order in which the elements are expected to appear in the list. */
static void expect_id_element_in_list(uint8_t id)
{
    EAS_ASSERT(num_expected_elements_in_list < LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS);
    expected_elements_in_list[num_expected_elements_in_list] = id;
    num_expected_elements_in_list++;
}

static void actual_id_element_detected_in_list(uint8_t id)
{
    EAS_ASSERT(num_actual_elements_in_list < LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS);
    actual_elements_in_list[num_actual_elements_in_list] = id;
    num_actual_elements_in_list++;
}

static void reset_expected_actual_id_elements()
{
    /* These arrays will contain ids, and 0 is a valid id. Set it to 0xFF (invalid id) to avoid false positives. */
    memset(expected_elements_in_list, 0xFF, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(uint8_t));
    memset(actual_elements_in_list, 0xFF, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(uint8_t));
    num_actual_elements_in_list = 0;
    num_expected_elements_in_list = 0;
}

static void verify_expected_id_elements()
{
    bool expected_elements_match_actual = (memcmp(expected_elements_in_list, actual_elements_in_list,
                                                  LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(uint8_t)) == 0);
    CHECK_TRUE(expected_elements_match_actual);
}

static void for_each_cb_id_elements(void *element, void *user_data)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    actual_id_element_detected_in_list(id_element->id);
}

static bool condition_id_element_cb(void *element, void *user_data)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    return id_element->condition_evaluation_result;
}

static void *actual_user_data = 0;
static void save_actual_user_data_cb(void *element, void *user_data)
{
    actual_user_data = user_data;
}

static void *remove_on_condition_actual_user_data = 0;
static bool remove_on_condition_save_use_data_cb(void *element, void *user_data)
{
    remove_on_condition_actual_user_data = user_data;
    return false;
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

TEST(LinkedList, PrependRaisesAssertIfListIsNull)
{
    LinkedListIdElement id_element_0 = {.id = 0};

    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_prepend");
    linked_list_prepend(NULL, &id_element_0);
}

TEST(LinkedList, ForEachRaisesAssertIfListIsNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_for_each");
    linked_list_for_each(NULL, for_each_cb_id_elements, NULL);
}

TEST(LinkedList, ForEachRaisesAssertIfCbIsNUll)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("cb", "linked_list_for_each");
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);

    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_for_each(linked_list, NULL, NULL);
}

TEST(LinkedList, RemoveOnConditionRaisesAssertIfListIsNUll)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_remove_on_condition");
    linked_list_remove_on_condition(NULL, condition_id_element_cb, NULL);
}

TEST(LinkedList, RemoveOnConditionRaisesAssertIfCbIsNUll)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("cb", "linked_list_remove_on_condition");
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);

    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, NULL, NULL);
}

TEST(LinkedList, RemoveRaisesAssertIfListIsNUll)
{
    LinkedListIdElement element = {.id = 0};
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_remove");

    linked_list_remove(NULL, &element);
}

TEST(LinkedList, PrependFiresAssertIfFailedToAllocateNode)
{
    LinkedList linked_list = linked_list_create();
    LinkedListIdElement element = {.id = 0};

    /* linked_list_node_allocator_alloc returns NULL if it fails to allocate the node. We expect linked_list_prepend to
     * detect this and fire an assert. */
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue((LinkedListNode *)NULL);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("new_node", "linked_list_prepend");

    linked_list_prepend(linked_list, &element);
}

TEST(LinkedList, AppendFiresAssertIfFailedToAllocateNode)
{
    LinkedList linked_list = linked_list_create();
    LinkedListIdElement element = {.id = 0};

    /* linked_list_node_allocator_alloc returns NULL if it fails to allocate the node. We expect linked_list_append to
     * detect this and fire an assert. */
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue((LinkedListNode *)NULL);
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("new_node", "linked_list_append");

    linked_list_append(linked_list, &element);
}

TEST(LinkedList, AppendRaisesAssertIfListIsNull)
{
    LinkedListIdElement id_element_0 = {.id = 0};

    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_append");
    linked_list_append(NULL, &id_element_0);
}

TEST(LinkedList, ListEmptyAfterCreate)
{
    LinkedList linked_list = linked_list_create();
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, ElementIsTheOnlyInListAfterPrepend)
{
    LinkedListIdElement id_element_3 = {.id = 3};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    expect_id_element_in_list(3);

    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_3);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    verify_expected_id_elements();
}

TEST(LinkedList, TwoElementsInListAfterPrependingTwoElements)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_2 = {.id = 2};
    expect_id_element_in_list(2);
    expect_id_element_in_list(0);

    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    verify_expected_id_elements();
}

TEST(LinkedList, ForEachPassesUserDataToCallback)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);

    LinkedList linked_list = linked_list_create();
    uint8_t expected_user_data = 42;
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_for_each(linked_list, save_actual_user_data_cb, &expected_user_data);

    CHECK_EQUAL(&expected_user_data, actual_user_data);
}

TEST(LinkedList, RemoveOnConditionEmptyListStillEmpty)
{
    LinkedList linked_list = linked_list_create();
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionDoesNotRemoveElementConditionFalse)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    /* Since condition evaluates to false, we expect remove_on_condition to keep it in the list. */
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionRemovesElementConditionTrue)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = true};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    /* We expect the list to be empty, so we are not calling expect_id_element_in_list here. */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionRemovesAllElementsConditionTrue)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = true};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = true};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = true};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_2_node);
    /* We expect the list to be empty, so we are not calling expect_id_element_in_list here. */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionKeepsAllElementsConditionFalse)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    /* We expect all three elements to be in the list, since condition is false for all of them. */
    expect_id_element_in_list(2);
    expect_id_element_in_list(1);
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
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
    expect_id_element_in_list(3);
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_prepend(linked_list, &id_element_3);
    linked_list_prepend(linked_list, &id_element_4);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionRemovesOnlyLastElement)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = true};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    expect_id_element_in_list(2);
    expect_id_element_in_list(1);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionRemovesOnlyFirstElement)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = true};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_2_node);
    expect_id_element_in_list(1);
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionRemovesOnlyMiddleElement)
{
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_4 = {.id = 4, .condition_evaluation_result = true};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_3 = {.id = 3, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_4_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_4_node);
    expect_id_element_in_list(3);
    expect_id_element_in_list(1);
    expect_id_element_in_list(0);
    expect_id_element_in_list(2);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_4);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_3);
    linked_list_remove_on_condition(linked_list, condition_id_element_cb, NULL);

    /* Verify */
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOnConditionPassesUserDataToCallback)
{
    LinkedListIdElement id_element_0 = {.id = 0};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);

    LinkedList linked_list = linked_list_create();
    uint8_t expected_user_data = 24;
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_remove_on_condition(linked_list, remove_on_condition_save_use_data_cb, &expected_user_data);

    CHECK_EQUAL(&expected_user_data, remove_on_condition_actual_user_data);
}

TEST(LinkedList, RemoveOnlyElement)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    /* Not expecting any elements in the list */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    bool is_removed = linked_list_remove(linked_list, &id_element_0);

    /* Verify */
    CHECK_TRUE(is_removed);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveTwoElements)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    /* Not expecting any elements in the list */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    bool is_removed_0 = linked_list_remove(linked_list, &id_element_0);
    bool is_removed_1 = linked_list_remove(linked_list, &id_element_1);

    /* Verify */
    CHECK_TRUE(is_removed_0);
    CHECK_TRUE(is_removed_1);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveOneOfTwoElements)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    expect_id_element_in_list(1);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    bool is_removed = linked_list_remove(linked_list, &id_element_0);

    /* Verify */
    CHECK_TRUE(is_removed);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveRemovesOnlyOneOfTwoIdenticalElements)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    /* Using id_element_1_node memory for the second copy of id_element_0 element in the list */
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    /* linked_list_remove should remove the first element in the list that matches the requested one to remove. Since
     * linked_list_prepend adds the element to the beginning of the list, we expect the memory of id_element_1_node to
     * be freed, since it is added to the list last. */
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    expect_id_element_in_list(0);

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_0);
    bool is_removed = linked_list_remove(linked_list, &id_element_0);

    /* Verify */
    CHECK_TRUE(is_removed);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveInRandomOrder)
{
    /* To ensure that the linked list removes the elements in the requested order - calls to
     * linked_list_node_allocator_free should be in the order they are specified */
    mock().strictOrder();

    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    LinkedListIdElement id_element_3 = {.id = 3, .condition_evaluation_result = false};
    LinkedListIdElement id_element_4 = {.id = 4, .condition_evaluation_result = false};
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_4_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_4_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_3_node);
    mock().expectOneCall("linked_list_node_allocator_free").withParameter("linked_list_node", id_element_1_node);
    /* Not expecting any elements in the list - all should get removed */

    /* Exercise */
    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    linked_list_prepend(linked_list, &id_element_2);
    linked_list_prepend(linked_list, &id_element_3);
    linked_list_prepend(linked_list, &id_element_4);
    bool all_removed = true;
    all_removed = all_removed && linked_list_remove(linked_list, &id_element_2);
    all_removed = all_removed && linked_list_remove(linked_list, &id_element_4);
    all_removed = all_removed && linked_list_remove(linked_list, &id_element_0);
    all_removed = all_removed && linked_list_remove(linked_list, &id_element_3);
    all_removed = all_removed && linked_list_remove(linked_list, &id_element_1);

    /* Verify */
    CHECK_TRUE(all_removed);
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, RemoveReturnsFalseEmptyList)
{
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};

    LinkedList linked_list = linked_list_create();
    bool is_removed = linked_list_remove(linked_list, &id_element_0);

    CHECK_FALSE(is_removed);
}

TEST(LinkedList, RemoveReturnsFalseElementNotInList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 0, .condition_evaluation_result = false};

    LinkedList linked_list = linked_list_create();
    linked_list_prepend(linked_list, &id_element_0);
    linked_list_prepend(linked_list, &id_element_1);
    bool is_removed = linked_list_remove(linked_list, &id_element_2);

    CHECK_FALSE(is_removed);
}

TEST(LinkedList, AppendAddsOneElementToEmptyList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    expect_id_element_in_list(0);

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);

    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, AppendAddsTwoElementsToEmptyList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    expect_id_element_in_list(0);
    expect_id_element_in_list(1);

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);
    linked_list_append(linked_list, &id_element_1);

    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, AppendAddsThreeElementsToEmptyList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    expect_id_element_in_list(0);
    expect_id_element_in_list(1);
    expect_id_element_in_list(2);

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);
    linked_list_append(linked_list, &id_element_1);
    linked_list_append(linked_list, &id_element_2);

    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);
    verify_expected_id_elements();
}

TEST(LinkedList, IteratorEmptyList)
{
    LinkedList linked_list = linked_list_create();
    void *iterator = linked_list_iterator_init(linked_list);
    void **element;
    bool is_valid_element = linked_list_iterator_next(linked_list, &iterator, element);

    /* There are 0 elements in the list, so the first call to iterator_next should signal the end of the list. */
    CHECK_FALSE(is_valid_element);
}

TEST(LinkedList, IteratorOneElementInList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement *retrieved_element_0;
    LinkedListIdElement *retrieved_element_1;

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);
    void *iterator = linked_list_iterator_init(linked_list);
    bool is_valid_element_0 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_0);
    bool is_valid_element_1 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_1);

    CHECK_TRUE(is_valid_element_0);
    CHECK_FALSE(is_valid_element_1);
    CHECK_EQUAL(&id_element_0, retrieved_element_0);
}

TEST(LinkedList, IteratorFourElementsInList)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_3_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    LinkedListIdElement id_element_3 = {.id = 3, .condition_evaluation_result = false};
    LinkedListIdElement *retrieved_element_0;
    LinkedListIdElement *retrieved_element_1;
    LinkedListIdElement *retrieved_element_2;
    LinkedListIdElement *retrieved_element_3;
    LinkedListIdElement *retrieved_element_4;

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);
    linked_list_append(linked_list, &id_element_1);
    linked_list_append(linked_list, &id_element_2);
    linked_list_append(linked_list, &id_element_3);
    void *iterator = linked_list_iterator_init(linked_list);
    bool is_valid_element_0 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_0);
    bool is_valid_element_1 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_1);
    bool is_valid_element_2 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_2);
    bool is_valid_element_3 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_3);
    /* All 4 elements have been iterated, we reached the end of the list. Should return false. */
    bool is_valid_element_4 = linked_list_iterator_next(linked_list, &iterator, (void **)&retrieved_element_4);

    CHECK_TRUE(is_valid_element_0);
    CHECK_TRUE(is_valid_element_1);
    CHECK_TRUE(is_valid_element_2);
    CHECK_TRUE(is_valid_element_3);
    CHECK_FALSE(is_valid_element_4);
    CHECK_EQUAL(&id_element_0, retrieved_element_0);
    CHECK_EQUAL(&id_element_1, retrieved_element_1);
    CHECK_EQUAL(&id_element_2, retrieved_element_2);
    CHECK_EQUAL(&id_element_3, retrieved_element_3);
}

TEST(LinkedList, TwoSimultaneousIterators)
{
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_0_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_1_node);
    mock().expectOneCall("linked_list_node_allocator_alloc").andReturnValue(id_element_2_node);
    LinkedListIdElement id_element_0 = {.id = 0, .condition_evaluation_result = false};
    LinkedListIdElement id_element_1 = {.id = 1, .condition_evaluation_result = false};
    LinkedListIdElement id_element_2 = {.id = 2, .condition_evaluation_result = false};
    LinkedListIdElement *iterator_0_element_0;
    LinkedListIdElement *iterator_0_element_1;
    LinkedListIdElement *iterator_0_element_2;
    LinkedListIdElement *iterator_0_element_3;
    LinkedListIdElement *iterator_1_element_0;
    LinkedListIdElement *iterator_1_element_1;
    LinkedListIdElement *iterator_1_element_2;
    LinkedListIdElement *iterator_1_element_3;

    LinkedList linked_list = linked_list_create();
    linked_list_append(linked_list, &id_element_0);
    linked_list_append(linked_list, &id_element_1);
    linked_list_append(linked_list, &id_element_2);
    void *iterator_0 = linked_list_iterator_init(linked_list);
    /* Iterate over the first two elements with iterator 0. */
    bool iter_0_element_0_valid = linked_list_iterator_next(linked_list, &iterator_0, (void **)&iterator_0_element_0);
    bool iter_0_element_1_valid = linked_list_iterator_next(linked_list, &iterator_0, (void **)&iterator_0_element_1);
    /* First element with iterator 1. */
    void *iterator_1 = linked_list_iterator_init(linked_list);
    bool iter_1_element_0_valid = linked_list_iterator_next(linked_list, &iterator_1, (void **)&iterator_1_element_0);
    /* Finish iteration with iterator 0. */
    bool iter_0_element_2_valid = linked_list_iterator_next(linked_list, &iterator_0, (void **)&iterator_0_element_2);
    bool iter_0_element_3_valid = linked_list_iterator_next(linked_list, &iterator_0, (void **)&iterator_0_element_3);
    /* Finish iteration with iterator 1. */
    bool iter_1_element_1_valid = linked_list_iterator_next(linked_list, &iterator_1, (void **)&iterator_1_element_1);
    bool iter_1_element_2_valid = linked_list_iterator_next(linked_list, &iterator_1, (void **)&iterator_1_element_2);
    bool iter_1_element_3_valid = linked_list_iterator_next(linked_list, &iterator_1, (void **)&iterator_1_element_3);

    /* Validate iteration with iterator 0. */
    CHECK_TRUE(iter_0_element_0_valid);
    CHECK_TRUE(iter_0_element_1_valid);
    CHECK_TRUE(iter_0_element_2_valid);
    CHECK_FALSE(iter_0_element_3_valid);
    CHECK_EQUAL(&id_element_0, iterator_0_element_0);
    CHECK_EQUAL(&id_element_1, iterator_0_element_1);
    CHECK_EQUAL(&id_element_2, iterator_0_element_2);
    /* Validate iteration with iterator 1. */
    CHECK_TRUE(iter_1_element_0_valid);
    CHECK_TRUE(iter_1_element_1_valid);
    CHECK_TRUE(iter_1_element_2_valid);
    CHECK_FALSE(iter_1_element_3_valid);
    CHECK_EQUAL(&id_element_0, iterator_1_element_0);
    CHECK_EQUAL(&id_element_1, iterator_1_element_1);
    CHECK_EQUAL(&id_element_2, iterator_1_element_2);
}

TEST(LinkedList, IteratorInitAssertsIfListIsNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("self", "linked_list_iterator_init");
    void *iterator = linked_list_iterator_init(NULL);
}
