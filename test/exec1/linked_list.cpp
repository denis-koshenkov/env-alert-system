#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "utils/linked_list.h"
#include "config.h"

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

TEST(LinkedList, elementIsTheOnlyInListAfterAdding)
{
    LinkedList linked_list = linked_list_create();

    linked_list_add(linked_list, &test_element);

    test_element.count = 0;
    linked_list_for_each(linked_list, for_each_cb_increment_count, NULL);

    CHECK_EQUAL(1, test_element.count);
}
