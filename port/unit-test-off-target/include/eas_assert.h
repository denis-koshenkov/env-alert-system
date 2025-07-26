#ifndef PORT_UNIT_TEST_OFF_TARGET_INCLUDE_EAS_ASSERT_H
#define PORT_UNIT_TEST_OFF_TARGET_INCLUDE_EAS_ASSERT_H

#include "CppUTest/TestHarness_c.h"

/* Fail the test if an assertion fails. We are currently not testing scenarios when assertions are expected to fail.
Using the C (not C++) macro from CppUTest, because this is used in production code which is compiled as C. */
#define EAS_ASSERT(expr) ((expr) ? (void)(0) : FAIL_TEXT_C("Assert failed"))

#endif
