#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTestExt/TestAssertPlugin.h"

int main(int ac, char **av)
{
    /* Test assert plugin */
    TestAssertPlugin testAssertPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&testAssertPlugin);

    /* Mock support plugin */
    MockSupportPlugin mockPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    return CommandLineTestRunner::RunAllTests(ac, av);
}
