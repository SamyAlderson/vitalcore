/**
 * @file test_thresholds.c
 * @brief Tests for clinical threshold evaluation.
 */

#include "test_framework.h"
#include "vitalcore/thresholds.h"

const float infant_bradycardia_threshold = 80.0f;
const float spo2_severe_hypoxia_threshold = 85.0f;
const float systolic_hypertensive_crisis_threshold = 200.0f;
const float temperature_hyperpyrexia_threshold = 41.5f;
const float temperature_hypothermia_threshold = 33.0f;

TEST(test_hr_normal_adult) {
    vc_threshold_result_t r = vc_eval_heart_rate(72.0f, VC_AGE_ADULT);
    ASSERT_FALSE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
    ASSERT_EQ(r.normal_low, 60.0f);
    ASSERT_EQ(r.normal_high, 100.0f);
}

TEST(test_hr_bradycardia_adult) {
    vc_threshold_result_t r = vc_eval_heart_rate(35.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
    ASSERT_STR_CONTAINS(r.interpretation, "bradycardia");
}

TEST(test_hr_tachycardia_adult) {
    vc_threshold_result_t r = vc_eval_heart_rate(140.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
    ASSERT_STR_CONTAINS(r.interpretation, "tachycardia");
}

TEST(test_hr_bradycardia_mild) {
    vc_threshold_result_t r = vc_eval_heart_rate(55.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_hr_normal_infant) {
    vc_threshold_result_t r = vc_eval_heart_rate(130.0f, VC_AGE_INFANT);
    ASSERT_FALSE(r.is_abnormal);
}

TEST(test_hr_bradycardia_infant) {
    vc_threshold_result_t r = vc_eval_heart_rate(infant_bradycardia_threshold, VC_AGE_INFANT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_spo2_normal) {
    vc_threshold_result_t r = vc_eval_spo2(98.0f, VC_AGE_ADULT);
    ASSERT_FALSE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_spo2_mild_hypoxia) {
    vc_threshold_result_t r = vc_eval_spo2(93.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_spo2_severe_hypoxia) {
    vc_threshold_result_t r = vc_eval_spo2(spo2_severe_hypoxia_threshold, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
    ASSERT_STR_CONTAINS(r.interpretation, "hypoxia");
}

TEST(test_systolic_normal) {
    vc_threshold_result_t r = vc_eval_systolic(120.0f, VC_AGE_ADULT);
    ASSERT_FALSE(r.is_abnormal);
}

TEST(test_systolic_hypotension) {
    vc_threshold_result_t r = vc_eval_systolic(80.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_systolic_hypertensive_crisis) {
    vc_threshold_result_t r = vc_eval_systolic(systolic_hypertensive_crisis_threshold, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}

TEST(test_temperature_normal) {
    vc_threshold_result_t r = vc_eval_temperature(36.8f);
    ASSERT_FALSE(r.is_abnormal);
}

TEST(test_temperature_fever) {
    vc_threshold_result_t r = vc_eval_temperature(38.8f);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
    ASSERT_STR_CONTAINS(r.interpretation, "Fever");
}

TEST(test_temperature_hyperpyrexia) {
    vc_threshold_result_t r = vc_eval_temperature(temperature_hyperpyrexia_threshold);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}

TEST(test_temperature_hypothermia) {
    vc_threshold_result_t r = vc_eval_temperature(temperature_hypothermia_threshold);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}