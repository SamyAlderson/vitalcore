/**
 * @file test_integration.c
 * @brief Integration tests for the full VitalCore monitor API.
 */

#include "test_framework.h"
#include "vitalcore/vitalcore.h"

TEST(test_version) {
    const char *v = vc_version();
    ASSERT_TRUE(v != NULL);
    ASSERT_STR_CONTAINS(v, "1.0.0");
}

TEST(test_monitor_create_destroy) {
    vc_monitor_t *m = vc_monitor_create();
    ASSERT_TRUE(m != NULL);
    vc_monitor_destroy(m);
}

TEST(test_monitor_custom_create) {
    vc_monitor_t *m = vc_monitor_create_custom(32, 15);
    ASSERT_TRUE(m != NULL);
    vc_monitor_destroy(m);
}

TEST(test_monitor_analyze_normal) {
    vc_monitor_t *m = vc_monitor_create();
    ASSERT_TRUE(m != NULL);

    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 72.0f;
    v.spo2 = 98.0f;
    v.systolic = 120.0f;
    v.diastolic = 80.0f;
    v.temperature = 36.8f;
    v.respiratory_rate = 14.0f;

    vc_alert_t alert;
    vc_severity_t sev = vc_monitor_analyze(m, &v, &alert);

    ASSERT_EQ(sev, VC_SEVERITY_INFO);
    ASSERT_TRUE(vc_monitor_get_mews_score(m) <= 1);
    ASSERT_TRUE(vc_monitor_get_risk_score(m) < 10.0f);

    vc_monitor_destroy(m);
}

TEST(test_monitor_analyze_critical) {
    vc_monitor_t *m = vc_monitor_create();
    ASSERT_TRUE(m != NULL);

    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 150.0f;
    v.spo2 = 85.0f;
    v.systolic = 70.0f;
    v.temperature = 40.5f;

    vc_alert_t alert;
    vc_severity_t sev = vc_monitor_analyze(m, &v, &alert);

    ASSERT_TRUE(sev >= VC_SEVERITY_CRITICAL);
    ASSERT_TRUE(vc_monitor_get_mews_score(m) >= 5);

    vc_monitor_destroy(m);
}

TEST(test_monitor_history_tracking) {
    vc_monitor_t *m = vc_monitor_create_custom(10, 30);

    for (int i = 0; i < 5; i++) {
        vc_vitals_t v = vc_vitals_default();
        v.heart_rate = 70.0f + (float)(i * 5);
        v.spo2 = 97.0f;
        v.systolic = 120.0f;
        v.diastolic = 80.0f;
        v.temperature = 36.8f;
        v.respiratory_rate = 16.0f;

        vc_alert_t alert;
        vc_monitor_analyze(m, &v, &alert);
    }

    const vc_vitals_history_t *h = vc_monitor_get_history(m);
    ASSERT_TRUE(h != NULL);
    ASSERT_EQ(h->count, 5u);

    vc_monitor_destroy(m);
}

TEST(test_monitor_alert_config) {
    vc_monitor_t *m = vc_monitor_create();
    vc_alert_config_t config = vc_alert_config_default();
    config.min_severity = VC_SEVERITY_WARNING;
    vc_monitor_set_alert_config(m, &config);

    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 55.0f; /* Mild bradycardia */

    vc_alert_t alert;
    vc_severity_t sev = vc_monitor_analyze(m, &v, &alert);
    /* Should still report even with WARNING threshold since severity is WARNING */
    ASSERT_TRUE(sev >= VC_SEVERITY_WARNING);

    vc_monitor_destroy(m);
}

TEST(test_monitor_emergency_workflow) {
    vc_monitor_t *m = vc_monitor_create();
    ASSERT_TRUE(m != NULL);

    /* Simulate a deteriorating patient */
    vc_vitals_t v1 = vc_vitals_default();
    v1.heart_rate = 80.0f;
    v1.spo2 = 97.0f;
    v1.systolic = 110.0f;
    v1.diastolic = 70.0f;
    v1.temperature = 36.8f;
    v1.respiratory_rate = 16.0f;

    vc_alert_t alert;
    vc_severity_t sev = vc_monitor_analyze(m, &v1, &alert);
    ASSERT_EQ(sev, VC_SEVERITY_INFO);

    /* Patient deteriorates */
    vc_vitals_t v2 = vc_vitals_default();
    v2.heart_rate = 130.0f;
    v2.spo2 = 82.0f;
    v2.systolic = 75.0f;
    v2.temperature = 39.5f;
    v2.respiratory_rate = 32.0f;

    sev = vc_monitor_analyze(m, &v2, &alert);
    ASSERT_TRUE(sev >= VC_SEVERITY_CRITICAL);

    /* Verify JSON output works */
    char json[2048];
    uint32_t len = vc_alert_format_json(&alert, json, sizeof(json));
    ASSERT_TRUE(len > 0);
    ASSERT_STR_CONTAINS(json, "\"severity\":");

    vc_monitor_destroy(m);
}

TEST(test_monitor_null_safety) {
    ASSERT_TRUE(vc_monitor_create() != NULL);
    vc_monitor_destroy(NULL); /* Should not crash */
    ASSERT_EQ(vc_monitor_get_risk_score(NULL), 0.0f);
    ASSERT_EQ(vc_monitor_get_mews_score(NULL), 0);
    ASSERT_TRUE(vc_monitor_get_history(NULL) == NULL);
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_version);
    RUN_TEST(test_monitor_create_destroy);
    RUN_TEST(test_monitor_custom_create);
    RUN_TEST(test_monitor_analyze_normal);
    RUN_TEST(test_monitor_analyze_critical);
    RUN_TEST(test_monitor_history_tracking);
    RUN_TEST(test_monitor_alert_config);
    RUN_TEST(test_monitor_emergency_workflow);
    RUN_TEST(test_monitor_null_safety);
    TEST_SUITE_END();
}
