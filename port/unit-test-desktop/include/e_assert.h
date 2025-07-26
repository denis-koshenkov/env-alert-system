#ifndef UTIL_E_ASSERT_H
#define UTIL_E_ASSERT_H

#include <stdio.h>
#include "CppUTestExt/MockSupport_c.h"

void on_assert() {
   fprintf(stdout, "inside on_assert\n");
   mock_c()->actualCall("on_assert");
   return;
}

#define E_ASSERT(expr)        \
    ((expr)								\
     ? (void)(0)						\
     : on_assert())

#endif
