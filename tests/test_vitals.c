/**
 * @file test_vitals.c
 * @brief Tests for vital signs data structures.
 */

#include "test_framework.h"
#include "vitalcore/vitals.h"

TEST(test_vitals_default_is_zeroed) {
    vc_vitals_t v = vc_vitals_default();
    ASSERT_EQ(v.heart_rate, 0.0f);
    ASSERT_EQ(v.spo2, 0.0f);
    ASSERT_EQ(v.systolic, 0.0f);
    ASSERT_EQ(v.diastolic, 0.0f);
    ASSERT_EQ(v.temperature, 0.0f);
    ASSERT_EQ(v.respiratory_rate, 0.0f);
    ASSERT_EQ(v.age, VC_AGE_ADULT);
    ASSERT_EQ(v.gender, VC_GENDER_UNKNOWN);
}

TEST(test_vitals_is_present) {
    ASSERT_TRUE(vc_vitals_is_present(72.0f));
    ASSERT_TRUE(vc_vitals_is_present(0.1f));
    ASSERT_FALSE(vc_vitals_is_present(0.0f));
    ASSERT_FALSE(vc_vitals_is_present(-1.0f));
}

TEST(test_calculate_map) {
    /* MAP = DBP + (SBP - DBP) / 3 */
    float map = vc_calculate_map(120.0f, 80.0f);
    ASSERT_NEAR(map, 93.33f, 0.1f);

    map = vc_calculate_map(100.0f, 60.0f);
    ASSERT_NEAR(map, 73.33f, 0.1f);

    map = vc_calculate_map(0.0f, 0.0f);
    ASSERT_EQ(map, 0.0f);
}

TEST(test_vitals_history_create) {
    vc_vitals_history_t *h = vc_vitals_history_create(10, 30);
    ASSERT_TRUE(h != NULL);
    ASSERT_EQ(h->count, 0u);
    ASSERT_EQ(h->capacity, 10u);
    ASSERT_EQ(h->window_minutes, 30u);
    vc_vitals_history_destroy(h);
}

TEST(test_vitals_history_null) {
    vc_vitals_history_t *h = vc_vitals_history_create(0, 30);
    ASSERT_TRUE(h == NULL);
}

TEST(test_vitals_history_add_and_get) {
    vc_vitals_history_t *h = vc_vitals_history_create(5, 30);
    ASSERT_TRUE(h != NULL);

    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 72.0f;
    v.timestamp = 1000;

    ASSERT_TRUE(vc_vitals_history_add(h, &v));
    ASSERT_EQ(h->count, 1u);

    const vc_vitals_t *latest = vc_vitals_history_latest(h);
    ASSERT_TRUE(latest != NULL);
    ASSERT_EQ(latest->heart_rate, 72.0f);

    vc_vitals_history_destroy(h);
}

TEST(test_vitals_history_overflow) {
    vc_vitals_history_t *h = vc_vitals_history_create(3, 30);
    ASSERT_TRUE(h != NULL);

    for (int i = 0; i < 5; i++) {
        vc_vitals_t v = vc_vitals_default();
        v.heart_rate = (float)(60 + i * 10);
        v.timestamp = 1000 + i;
        vc_vitals_history_add(h, &v);
    }

    ASSERT_EQ(h->count, 3u);
    const vc_vitals_t *latest = vc_vitals_history_latest(h);
    ASSERT_EQ(latest->heart_rate, 100.0f); /* Last added */

    vc_vitals_history_destroy(h);
}

TEST(test_vitals_history_empty_latest) {
    vc_vitals_history_t *h = vc_vitals_history_create(5, 30);
    ASSERT_TRUE(vc_vitals_history_latest(h) == NULL);
    vc_vitals_history_destroy(h);
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_vitals_default_is_zeroed);
    RUN_TEST(test_vitals_is_present);
    RUN_TEST(test_calculate_map);
    RUN_TEST(test_vitals_history_create);
    RUN_TEST(test_vitals_history_null);
    RUN_TEST(test_vitals_history_add_and_get);
    RUN_TEST(test_vitals_history_overflow);
    RUN_TEST(test_vitals_history_empty_latest);
    TEST_SUITE_END();
}
