/**
 * @file test_framework.h
 * @brief Minimal test framework for VitalCore.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

static int _tests_run = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;
static int _current_test_failed = 0;

#define TEST(name) static void name(void)

#define RUN_TEST(name) do { \
    _current_test_failed = 0; \
    _tests_run++; \
    printf("  [RUN ] %s\n", #name); \
    name(); \
    if (_current_test_failed) { \
        _tests_failed++; \
        printf("  [FAIL] %s\n", #name); \
    } else { \
        _tests_passed++; \
        printf("  [ OK ] %s\n", #name); \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("    ASSERT_TRUE failed: %s (line %d)\n", #cond, __LINE__); \
        _current_test_failed = 1; \
        return; \
    } \
} while(0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf("    ASSERT_EQ failed: %s == %s (line %d)\n", #a, #b, __LINE__); \
        _current_test_failed = 1; \
        return; \
    } \
} while(0)

#define ASSERT_NEQ(a, b) do { \
    if ((a) == (b)) { \
        printf("    ASSERT_NEQ failed: %s != %s (line %d)\n", #a, #b, __LINE__); \
        _current_test_failed = 1; \
        return; \
    } \
} while(0)

#define ASSERT_NEAR(a, b, eps) do { \
    float _diff = (float)((a) - (b)); \
    if (_diff < 0) _diff = -_diff; \
    if (_diff > (eps)) { \
        printf("    ASSERT_NEAR failed: %s ~ %s (diff=%.4f, eps=%.4f, line %d)\n", \
               #a, #b, _diff, (float)(eps), __LINE__); \
        _current_test_failed = 1; \
        return; \
    } \
} while(0)

#define ASSERT_STR_CONTAINS(haystack, needle) do { \
    if (!strstr((haystack), (needle))) { \
        printf("    ASSERT_STR_CONTAINS failed: \"%s\" not in \"%s\" (line %d)\n", \
               (needle), (haystack), __LINE__); \
        _current_test_failed = 1; \
        return; \
    } \
} while(0)

#define TEST_SUITE_BEGIN() \
    do { \
        _tests_run = 0; _tests_passed = 0; _tests_failed = 0; \
        printf("\n========================================\n"); \
        printf("Test Suite: %s\n", __FILE__); \
        printf("========================================\n"); \
    } while(0)

#define TEST_SUITE_END() \
    do { \
        printf("\n----------------------------------------\n"); \
        printf("Results: %d/%d passed", _tests_passed, _tests_run); \
        if (_tests_failed > 0) printf(" (%d FAILED)", _tests_failed); \
        printf("\n----------------------------------------\n\n"); \
        return _tests_failed > 0 ? 1 : 0; \
    } while(0)

#endif /* TEST_FRAMEWORK_H */
