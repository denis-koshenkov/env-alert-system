#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/TestAssertPlugin.h"

int main(int ac, char **av)
{
    TestAssertPlugin testAssertPlugin;
    TestRegistry::getCurrentRegistry()->installPlugin(&testAssertPlugin);
    return CommandLineTestRunner::RunAllTests(ac, av);
}
