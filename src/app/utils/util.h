#ifndef ENV_ALERT_SYSTEM_SRC_APP_UTILS_UTIL_H
#define ENV_ALERT_SYSTEM_SRC_APP_UTILS_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Static assert that fires at compile time.
 *
 * Produces an error during compilation if condition evaluates to 0. Otherwise, nothing happens and no extra code is
 * generated.
 *
 * For C, the trick is that an array of negative size will not compile. If condition is 0, then we get: static_assert[1
 * - 2 * 1] <-> static_assert[-1], which throws a compilation error. If condition is 1, we get: static_assert[1 - 2 * 0]
 * <-> static_assert[1]. This will not cause a compilation error.
 *
 * If condition is 1, the macro expands to: extern int static_assert[1].
 * This will not generate any extra code, as it is just an external declaration.
 *
 * For C++, use built-in static_assert.
 */
#ifdef __cplusplus
#define EAS_STATIC_ASSERT(condition) static_assert(condition)
#else
#define EAS_STATIC_ASSERT(condition) extern int static_assert[1 - 2 * !(condition)]
#endif

/**
 * @brief Find maximum out of two values - a and b.
 *
 * @p a
 * @p b
 */
#define MAX2(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Find maximum out of four values - a, b, c, and d.
 *
 * @p a
 * @p b
 * @p c
 * @p d
 */
#define MAX4(a, b, c, d) MAX2(MAX2((a), (b)), MAX2((c), (d)))

/** Makes x divisible by 4 by increasing its value, if necessary. */
#define DIV4_UP(x) (((x) + 3) % 4)

#ifdef __cplusplus
}
#endif

#endif /* ENV_ALERT_SYSTEM_SRC_APP_UTILS_UTIL_H */
