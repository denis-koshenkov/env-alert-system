#ifndef UTIL_E_ASSERT_H
#define UTIL_E_ASSERT_H

void on_assert() {}

// TODO: decide what E_ASSERT does instead of calling nop() when we are in embedded
#define E_ASSERT(expr)        \
    ((expr)								\
     ? (void)(0)						\
     : on_assert())

#endif
