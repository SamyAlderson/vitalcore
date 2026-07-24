/**
 * @file test_alert.c
 * @brief Tests for alert engine.
 */

#include "test_framework.h"
#include "vitalcore/alert.h"

TEST(test_alert_config_default) {
    vc_alert_config_t config = vc_alert_config_default();
    ASSERT_EQ(config.format, VC_FORMAT_TEXT);
    ASSERT_EQ(config.min_severity, VC_SEVERITY_INFO);
    ASSERT_TRUE(config.include_details);
    ASSERT_TRUE(config.include_recommendation);
}

TEST(test_alert_normal_vitals) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 72.0f;
    v.spo2 = 98.0f;
    v.systolic = 120.0f;
    v.diastolic = 80.0f;
    v.temperature = 36.8f;
    v.respiratory_rate = 14.0f;

    vc_alert_t alert;
    vc_severity_t sev = vc_generate_alert(&v, NULL, NULL, &alert);

    ASSERT_EQ(sev, VC_SEVERITY_INFO);
    ASSERT_STR_CONTAINS(alert.message, "normal");
    ASSERT_TRUE(alert.mews_score <= 1);
    ASSERT_TRUE(alert.risk_score < 10.0f);
}

TEST(test_alert_critical_vitals) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 35.0f;
    v.spo2 = 82.0f;
    v.systolic = 65.0f;
    v.temperature = 33.0f;

    vc_alert_t alert;
    vc_severity_t sev = vc_generate_alert(&v, NULL, NULL, &alert);

    ASSERT_TRUE(sev >= VC_SEVERITY_CRITICAL);
    ASSERT_TRUE(alert.anomalies.count > 0);
    ASSERT_TRUE(alert.mews_score > 5);
}

TEST(test_alert_format_text) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 40.0f;

    vc_alert_t alert;
    vc_generate_alert(&v, NULL, NULL, &alert);

    char buffer[2048];
    uint32_t len = vc_alert_format_text(&alert, buffer, sizeof(buffer));

    ASSERT_TRUE(len > 0);
    ASSERT_STR_CONTAINS(buffer, "VITALCORE ALERT");
    ASSERT_STR_CONTAINS(buffer, "Risk Level:");
    ASSERT_STR_CONTAINS(buffer, "MEWS Score:");
}

TEST(test_alert_format_json) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 40.0f;

    vc_alert_t alert;
    vc_generate_alert(&v, NULL, NULL, &alert);

    char buffer[2048];
    uint32_t len = vc_alert_format_json(&alert, buffer, sizeof(buffer));

    ASSERT_TRUE(len > 0);
    ASSERT_STR_CONTAINS(buffer, "\"severity\":");
    ASSERT_STR_CONTAINS(buffer, "\"risk_level\":");
    ASSERT_STR_CONTAINS(buffer, "\"mews_score\":");
}

TEST(test_alert_emergency) {
    vc_vitals_t v = vc_vitals_default();
    v.spo2 = 75.0f; /* Severe hypoxia = emergency */

    vc_alert_t alert;
    vc_severity_t sev = vc_generate_alert(&v, NULL, NULL, &alert);

    ASSERT_EQ(sev, VC_SEVERITY_EMERGENCY);
    ASSERT_TRUE(alert.anomalies.has_emergency);
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_alert_config_default);
    RUN_TEST(test_alert_normal_vitals);
    RUN_TEST(test_alert_critical_vitals);
    RUN_TEST(test_alert_format_text);
    RUN_TEST(test_alert_format_json);
    RUN_TEST(test_alert_emergency);
    TEST_SUITE_END();
}
