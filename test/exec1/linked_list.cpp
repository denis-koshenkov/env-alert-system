#include "CppUTest/TestHarness.h"
#include "CppUTestExt/TestAssertPlugin.h"

#include "utils/linked_list.h"

struct LinkedListTestNode {
    uint8_t value;
    LinkedListNode node;
};

TEST_GROUP(LinkedList){};

TEST(LinkedList, ListEmptyAfterCreate)
{
    LinkedList linked_list;
    linked_list_create(&linked_list);

    size_t count = 0;
    LINKED_LIST_FOR_EACH(linked_list, LinkedListTestNode, node)
    {
        count++;
    }

    CHECK_EQUAL(0, count);
}

TEST(LinkedList, createRaisesAssertIfListIsNull)
{
    TEST_ASSERT_PLUGIN_EXPECT_ASSERTION("linked_list", "linked_list_create");
    linked_list_create(NULL);
}

TEST(LinkedList, elementIsTheOnlyInListAfterAdding)
{
    LinkedList linked_list;
    LinkedListTestNode test_node_1 = {
        .value = 42,
    };
    linked_list_create(&linked_list);

    linked_list_add(&linked_list, &(test_node_1.node));

    size_t count = 0;
    LINKED_LIST_FOR_EACH(linked_list, LinkedListTestNode, node)
    {
        count++;
    }

    CHECK_EQUAL(1, count);
}
