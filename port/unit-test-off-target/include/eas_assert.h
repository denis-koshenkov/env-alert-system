#ifndef PORT_UNIT_TEST_OFF_TARGET_INCLUDE_EAS_ASSERT_H
#define PORT_UNIT_TEST_OFF_TARGET_INCLUDE_EAS_ASSERT_H

#include "CppUTestExt/TestAssertPlugin_c.h"

#define EAS_ASSERT(expr) ((expr) ? (void)(0) : TEST_ASSERT_PLUGIN_ASSERT())

#endif
