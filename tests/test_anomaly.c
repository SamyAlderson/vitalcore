/**
 * @file test_anomaly.c
 * @brief Tests for anomaly detection.
 */

#include "test_framework.h"
#include "vitalcore/anomaly.h"

TEST(test_analyze_normal_vitals) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 72.0f;
    v.spo2 = 98.0f;
    v.systolic = 120.0f;
    v.diastolic = 80.0f;
    v.temperature = 36.8f;
    v.respiratory_rate = 16.0f;

    vc_anomaly_result_t result;
    uint32_t count = vc_analyze(&v, NULL, &result);

    ASSERT_EQ(count, 0u);
    ASSERT_FALSE(result.has_emergency);
    ASSERT_FALSE(result.has_critical);
}

TEST(test_analyze_bradycardia) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 35.0f;

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    ASSERT_TRUE(result.count > 0);
    ASSERT_EQ(result.anomalies[0].type, VC_ANOMALY_BRADYCARDIA);
    ASSERT_TRUE(result.anomalies[0].severity >= VC_SEVERITY_CRITICAL);
}

TEST(test_analyze_hypoxia) {
    vc_vitals_t v = vc_vitals_default();
    v.spo2 = 85.0f;

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    ASSERT_TRUE(result.count > 0);
    ASSERT_EQ(result.anomalies[0].type, VC_ANOMALY_HYPOXIA);
    ASSERT_TRUE(result.has_emergency);
}

TEST(test_analyze_hypotension) {
    vc_vitals_t v = vc_vitals_default();
    v.systolic = 65.0f;
    v.diastolic = 35.0f;

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    ASSERT_TRUE(result.count > 0);
    bool found_hypotension = false;
    for (uint32_t i = 0; i < result.count; i++) {
        if (result.anomalies[i].type == VC_ANOMALY_HYPOTENSION) {
            found_hypotension = true;
            break;
        }
    }
    ASSERT_TRUE(found_hypotension);
}

TEST(test_analyze_fever) {
    vc_vitals_t v = vc_vitals_default();
    v.temperature = 39.5f;

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    ASSERT_TRUE(result.count > 0);
    ASSERT_EQ(result.anomalies[0].type, VC_ANOMALY_FEVER);
}

TEST(test_analyze_shock_index) {
    vc_vitals_t v = vc_vitals_default();
    v.heart_rate = 120.0f;
    v.systolic = 80.0f; /* SI = 120/80 = 1.5 > 1.0 */

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    bool found_shock = false;
    for (uint32_t i = 0; i < result.count; i++) {
        if (result.anomalies[i].type == VC_ANOMALY_SHOCK_INDEX_HIGH) {
            found_shock = true;
            ASSERT_TRUE(result.anomalies[i].severity >= VC_SEVERITY_CRITICAL);
            break;
        }
    }
    ASSERT_TRUE(found_shock);
}

TEST(test_analyze_sepsis_screen) {
    vc_vitals_t v = vc_vitals_default();
    v.systolic = 85.0f;    /* hypotension */
    v.temperature = 39.0f; /* fever */
    v.heart_rate = 110.0f; /* tachycardia */

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    bool found_sepsis = false;
    for (uint32_t i = 0; i < result.count; i++) {
        if (result.anomalies[i].type == VC_ANOMALY_SEPSIS_SCREEN) {
            found_sepsis = true;
            break;
        }
    }
    ASSERT_TRUE(found_sepsis);
}

TEST(test_analyze_desat_tachypnea) {
    vc_vitals_t v = vc_vitals_default();
    v.spo2 = 90.0f;
    v.respiratory_rate = 30.0f;

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    bool found = false;
    for (uint32_t i = 0; i < result.count; i++) {
        if (result.anomalies[i].type == VC_ANOMALY_DESAT_TACHYPNEA) {
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found);
}

TEST(test_analyze_low_pulse_pressure) {
    vc_vitals_t v = vc_vitals_default();
    v.systolic = 90.0f;
    v.diastolic = 80.0f; /* PP = 10 < 25 */

    vc_anomaly_result_t result;
    vc_analyze(&v, NULL, &result);

    bool found = false;
    for (uint32_t i = 0; i < result.count; i++) {
        if (result.anomalies[i].type == VC_ANOMALY_PULSE_PRESSURE_LOW) {
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found);
}

TEST(test_anomaly_type_names) {
    ASSERT_STR_CONTAINS(vc_anomaly_type_name(VC_ANOMALY_BRADYCARDIA), "Bradycardia");
    ASSERT_STR_CONTAINS(vc_anomaly_type_name(VC_ANOMALY_HYPOXIA), "Hypoxia");
    ASSERT_STR_CONTAINS(vc_anomaly_type_name(VC_ANOMALY_FEVER), "Fever");
}

TEST(test_severity_names) {
    ASSERT_STR_CONTAINS(vc_severity_name(VC_SEVERITY_INFO), "INFO");
    ASSERT_STR_CONTAINS(vc_severity_name(VC_SEVERITY_WARNING), "WARNING");
    ASSERT_STR_CONTAINS(vc_severity_name(VC_SEVERITY_CRITICAL), "CRITICAL");
    ASSERT_STR_CONTAINS(vc_severity_name(VC_SEVERITY_EMERGENCY), "EMERGENCY");
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_analyze_normal_vitals);
    RUN_TEST(test_analyze_bradycardia);
    RUN_TEST(test_analyze_hypoxia);
    RUN_TEST(test_analyze_hypotension);
    RUN_TEST(test_analyze_fever);
    RUN_TEST(test_analyze_shock_index);
    RUN_TEST(test_analyze_sepsis_screen);
    RUN_TEST(test_analyze_desat_tachypnea);
    RUN_TEST(test_analyze_low_pulse_pressure);
    RUN_TEST(test_anomaly_type_names);
    RUN_TEST(test_severity_names);
    TEST_SUITE_END();
}
