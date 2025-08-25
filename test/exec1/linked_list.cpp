#include <string.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "utils/linked_list.h"
#include "config.h"

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

    linked_list_add(linked_list, &test_element);

    test_element.count = 0;
    linked_list_for_each(linked_list, for_each_cb_increment_count, NULL);

    CHECK_EQUAL(1, test_element.count);
}

#define LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS 10
static bool actual_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];
static bool expected_elements_in_list[LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS];

static void for_each_cb_id_elements(void *element, void *user_data)
{
    LinkedListIdElement *id_element = (LinkedListIdElement *)element;
    actual_elements_in_list[id_element->id] = true;
}

TEST(LinkedList, TwoElementsInListAfterAddingTwoElements)
{
    LinkedList linked_list = linked_list_create();

    LinkedListIdElement id_element_0 = {.id = 0};
    LinkedListIdElement id_element_2 = {.id = 2};

    linked_list_add(linked_list, &id_element_0);
    linked_list_add(linked_list, &id_element_2);

    memset(expected_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
    expected_elements_in_list[0] = true;
    expected_elements_in_list[2] = true;

    memset(actual_elements_in_list, 0, LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool));
    linked_list_for_each(linked_list, for_each_cb_id_elements, NULL);

    CHECK_TRUE(memcmp(expected_elements_in_list, actual_elements_in_list,
                      LINKED_LIST_TEST_MAX_NUM_ID_ELEMENTS * sizeof(bool)) == 0);
}
