/**
 * @file thresholds.h
 * @brief Clinical thresholds for vital signs evaluation.
 *
 * Based on WHO Emergency Triage guidelines and AHA recommendations.
 * Thresholds are age-group specific.
 */

#ifndef VITALCORE_THRESHOLDS_H
#define VITALCORE_THRESHOLDS_H

#include "vitals.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Threshold evaluation result for a single vital sign.
 */
typedef struct {
    const char *name;           /**< Name of the vital sign */
    float value;                /**< Measured value */
    float normal_low;           /**< Lower bound of normal range */
    float normal_high;          /**< Upper bound of normal range */
    bool is_abnormal;           /**< true if value is outside normal range */
    bool is_critical;           /**< true if value is in critical range */
    const char *interpretation; /**< Human-readable interpretation */
} vc_threshold_result_t;

/**
 * @brief Evaluate heart rate against clinical thresholds.
 *
 * @param heart_rate Heart rate in bpm.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_heart_rate(float heart_rate, vc_age_group_t age);

/**
 * @brief Evaluate SpO2 against clinical thresholds.
 *
 * @param spo2 Oxygen saturation in %.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_spo2(float spo2, vc_age_group_t age);

/**
 * @brief Evaluate systolic blood pressure against clinical thresholds.
 *
 * @param systolic Systolic pressure in mmHg.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_systolic(float systolic, vc_age_group_t age);

/**
 * @brief Evaluate diastolic blood pressure against clinical thresholds.
 *
 * @param diastolic Diastolic pressure in mmHg.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_diastolic(float diastolic, vc_age_group_t age);

/**
 * @brief Evaluate body temperature against clinical thresholds.
 *
 * @param temperature Temperature in °C.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_temperature(float temperature);

/**
 * @brief Evaluate respiratory rate against clinical thresholds.
 *
 * @param rr Respiratory rate in breaths/min.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_respiratory_rate(float rr, vc_age_group_t age);

#ifdef __cplusplus
}
#endif

#endif /* VITALCORE_THRESHOLDS_H */
