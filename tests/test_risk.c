/**
 * @file test_risk.c
 * @brief Tests for risk scoring.
 */

#include "test_framework.h"
#include "vitalcore/risk.h"

TEST(test_mews_normal) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 75.0f;
    v.systolic = 120.0f;
    v.respiratory_rate = 14.0f;
    v.temperature = 36.8f;

    vc_mews_score_t mews = vc_calculate_mews(&v);
    ASSERT_EQ(mews.total, 0);
    ASSERT_EQ(mews.risk_level, VC_RISK_LOW);
}

TEST(test_mews_elevated) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 125.0f;  /* score: 2 */
    v.systolic = 85.0f;     /* score: 1 */
    v.respiratory_rate = 25.0f; /* score: 2 */
    v.temperature = 36.8f;  /* score: 0 */

    vc_mews_score_t mews = vc_calculate_mews(&v);
    ASSERT_TRUE(mews.total >= 5);
    ASSERT_TRUE(mews.risk_level >= VC_RISK_HIGH);
}

TEST(test_mews_critical) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 35.0f;   /* score: 3 */
    v.systolic = 65.0f;     /* score: 3 */
    v.respiratory_rate = 6.0f; /* score: 3 */
    v.temperature = 34.0f;  /* score: 2 */

    vc_mews_score_t mews = vc_calculate_mews(&v);
    ASSERT_TRUE(mews.total >= 10);
    ASSERT_EQ(mews.risk_level, VC_RISK_CRITICAL);
}

TEST(test_risk_score_normal) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 72.0f;
    v.spo2 = 98.0f;
    v.systolic = 120.0f;
    v.diastolic = 80.0f;
    v.temperature = 36.8f;
    v.respiratory_rate = 16.0f;

    float score = vc_calculate_risk_score(&v, NULL, NULL);
    ASSERT_TRUE(score < 10.0f);
}

TEST(test_risk_score_critical) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 35.0f;
    v.spo2 = 82.0f;
    v.systolic = 65.0f;
    v.diastolic = 35.0f;
    v.temperature = 33.0f;
    v.respiratory_rate = 6.0f;

    float score = vc_calculate_risk_score(&v, NULL, NULL);
    ASSERT_TRUE(score > 30.0f);
}

TEST(test_risk_level_names) {
    ASSERT_STR_CONTAINS(vc_risk_level_name(VC_RISK_LOW), "LOW");
    ASSERT_STR_CONTAINS(vc_risk_level_name(VC_RISK_HIGH), "HIGH");
    ASSERT_STR_CONTAINS(vc_risk_level_name(VC_RISK_CRITICAL), "CRITICAL");
}

TEST(test_risk_level_actions) {
    const char *action = vc_risk_level_action(VC_RISK_LOW);
    ASSERT_TRUE(strlen(action) > 0);

    action = vc_risk_level_action(VC_RISK_CRITICAL);
    ASSERT_STR_CONTAINS(action, "Immediate");
}

TEST(test_risk_score_with_anomalies) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 130.0f;
    v.spo2 = 88.0f;
    v.systolic = 85.0f;

    vc_anomaly_result_t anomalies;
    vc_analyze(&v, NULL, &anomalies);

    float score = vc_calculate_risk_score(&v, NULL, &anomalies);
    ASSERT_TRUE(score > 20.0f);
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_mews_normal);
    RUN_TEST(test_mews_elevated);
    RUN_TEST(test_mews_critical);
    RUN_TEST(test_risk_score_normal);
    RUN_TEST(test_risk_score_critical);
    RUN_TEST(test_risk_level_names);
    RUN_TEST(test_risk_level_actions);
    RUN_TEST(test_risk_score_with_anomalies);
    TEST_SUITE_END();
}
