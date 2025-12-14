#include <stdbool.h>

#include <zephyr/kernel.h>

#include "osal/eas_thread.h"
#include "eas_assert.h"
#include "config.h"

#ifndef CONFIG_EAS_THREAD_STACK_SIZE
#define CONFIG_EAS_THREAD_STACK_SIZE 1024
#endif

#ifndef CONFIG_EAS_THREAD_PRIORITY
#define CONFIG_EAS_THREAD_PRIORITY 0
#endif

/* Zephyr-related definitions */
K_THREAD_STACK_DEFINE(eas_thread_stack_area, CONFIG_EAS_THREAD_STACK_SIZE);
static struct k_thread thread;

static EasThreadRunFunction eas_thread_run_function;
static bool is_created = false;

static void eas_thread_entry_point(void *, void *, void *)
{
    EAS_ASSERT(eas_thread_run_function);
    eas_thread_run_function();
}

void eas_thread_create(EasThreadRunFunction run_function)
{
    EAS_ASSERT(run_function);
    /* The implementation currently only supports creating one thread, assert if this function is called more than once
     */
    EAS_ASSERT(!is_created);

    eas_thread_run_function = run_function;
    is_created = true;
    k_thread_create(&thread, eas_thread_stack_area, K_THREAD_STACK_SIZEOF(eas_thread_stack_area),
                    eas_thread_entry_point, NULL, NULL, NULL, CONFIG_EAS_THREAD_PRIORITY, 0, K_NO_WAIT);
}
