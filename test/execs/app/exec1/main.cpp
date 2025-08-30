#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTestExt/TestAssertPlugin.h"

class VariableRequirementListForEachCbComparator : public MockNamedValueComparator {
  public:
    virtual bool isEqual(const void *object1, const void *object2)
    {
        return object1 == object2;
    }
    virtual SimpleString valueToString(const void *object)
    {
        return StringFrom(object);
    }
};

int main(int ac, char **av)
{
    /* Test assert plugin */
    TestAssertPlugin testAssertPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&testAssertPlugin);

    /* Mock support plugin */
    MockSupportPlugin mockPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    /* Install comparator for VariableRequirementListForEachCb type */
    VariableRequirementListForEachCbComparator variableRequirementListForEachCbComparator;
    mockPlugin.installComparator("VariableRequirementListForEachCb", variableRequirementListForEachCbComparator);

    return CommandLineTestRunner::RunAllTests(ac, av);
}
