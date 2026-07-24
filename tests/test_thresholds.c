/**
 * @file test_thresholds.c
 * @brief Tests for clinical threshold evaluation.
 */

#include "test_framework.h"
#include "vitalcore/thresholds.h"

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
    vc_threshold_result_t r = vc_eval_heart_rate(85.0f, VC_AGE_INFANT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical); /* 85 > 80 critical threshold */
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
    vc_threshold_result_t r = vc_eval_spo2(85.0f, VC_AGE_ADULT);
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
    vc_threshold_result_t r = vc_eval_systolic(200.0f, VC_AGE_ADULT);
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
    vc_threshold_result_t r = vc_eval_temperature(41.5f);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}

TEST(test_temperature_hypothermia) {
    vc_threshold_result_t r = vc_eval_temperature(33.0f);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}

TEST(test_rr_normal) {
    vc_threshold_result_t r = vc_eval_respiratory_rate(16.0f, VC_AGE_ADULT);
    ASSERT_FALSE(r.is_abnormal);
}

TEST(test_rr_tachypnea) {
    vc_threshold_result_t r = vc_eval_respiratory_rate(28.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_FALSE(r.is_critical);
}

TEST(test_rr_bradypnea_critical) {
    vc_threshold_result_t r = vc_eval_respiratory_rate(6.0f, VC_AGE_ADULT);
    ASSERT_TRUE(r.is_abnormal);
    ASSERT_TRUE(r.is_critical);
}

int main(void) {
    TEST_SUITE_BEGIN();
    RUN_TEST(test_hr_normal_adult);
    RUN_TEST(test_hr_bradycardia_adult);
    RUN_TEST(test_hr_tachycardia_adult);
    RUN_TEST(test_hr_bradycardia_mild);
    RUN_TEST(test_hr_normal_infant);
    RUN_TEST(test_hr_bradycardia_infant);
    RUN_TEST(test_spo2_normal);
    RUN_TEST(test_spo2_mild_hypoxia);
    RUN_TEST(test_spo2_severe_hypoxia);
    RUN_TEST(test_systolic_normal);
    RUN_TEST(test_systolic_hypotension);
    RUN_TEST(test_systolic_hypertensive_crisis);
    RUN_TEST(test_temperature_normal);
    RUN_TEST(test_temperature_fever);
    RUN_TEST(test_temperature_hyperpyrexia);
    RUN_TEST(test_temperature_hypothermia);
    RUN_TEST(test_rr_normal);
    RUN_TEST(test_rr_tachypnea);
    RUN_TEST(test_rr_bradypnea_critical);
    TEST_SUITE_END();
}
