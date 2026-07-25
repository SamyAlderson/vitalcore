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
vc_threshold_result_t vc_eval_heart_rate(float heart_rate, vc_age_group_t age) {
    // Define age-specific thresholds based on WHO Emergency Triage guidelines
    static const float heart_rate_thresholds[][2] = {
        {0, 100}, // Infant (0-1 year)
        {80, 120}, // Child (1-12 years)
        {60, 100}, // Adult (13-64 years)
        {50, 90}  // Elderly (65+ years)
    };

    // Check for invalid age group
    if (age < 0 || age >= sizeof(heart_rate_thresholds) / sizeof(heart_rate_thresholds[0])) {
        // Return error result
        vc_threshold_result_t result = {NULL, 0.0f, 0.0f, 0.0f, true, true, "Invalid age group"};
        return result;
    }

    // Evaluate heart rate against thresholds
    float normal_low = heart_rate_thresholds[age][0];
    float normal_high = heart_rate_thresholds[age][1];

    // Determine if heart rate is abnormal or critical
    bool is_abnormal = (heart_rate < normal_low || heart_rate > normal_high);
    bool is_critical = (heart_rate < normal_low - 20 || heart_rate > normal_high + 20);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "Heart Rate", heart_rate, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

/**
 * @brief Evaluate SpO2 against clinical thresholds.
 *
 * @param spo2 Oxygen saturation in %.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_spo2(float spo2, vc_age_group_t age) {
    // Define age-specific thresholds based on AHA recommendations
    static const float spo2_thresholds[][2] = {
        {90, 100}, // Infant (0-1 year)
        {90, 100}, // Child (1-12 years)
        {90, 100}, // Adult (13-64 years)
        {80, 100}  // Elderly (65+ years)
    };

    // Check for invalid age group
    if (age < 0 || age >= sizeof(spo2_thresholds) / sizeof(spo2_thresholds[0])) {
        // Return error result
        vc_threshold_result_t result = {NULL, 0.0f, 0.0f, 0.0f, true, true, "Invalid age group"};
        return result;
    }

    // Evaluate SpO2 against thresholds
    float normal_low = spo2_thresholds[age][0];
    float normal_high = spo2_thresholds[age][1];

    // Determine if SpO2 is abnormal or critical
    bool is_abnormal = (spo2 < normal_low || spo2 > normal_high);
    bool is_critical = (spo2 < normal_low - 10 || spo2 > normal_high + 10);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "SpO2", spo2, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

/**
 * @brief Evaluate systolic blood pressure against clinical thresholds.
 *
 * @param systolic Systolic pressure in mmHg.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_systolic(float systolic, vc_age_group_t age) {
    // Define age-specific thresholds based on WHO Emergency Triage guidelines
    static const float systolic_thresholds[][2] = {
        {60, 100}, // Infant (0-1 year)
        {70, 120}, // Child (1-12 years)
        {90, 140}, // Adult (13-64 years)
        {80, 130}  // Elderly (65+ years)
    };

    // Check for invalid age group
    if (age < 0 || age >= sizeof(systolic_thresholds) / sizeof(systolic_thresholds[0])) {
        // Return error result
        vc_threshold_result_t result = {NULL, 0.0f, 0.0f, 0.0f, true, true, "Invalid age group"};
        return result;
    }

    // Evaluate systolic blood pressure against thresholds
    float normal_low = systolic_thresholds[age][0];
    float normal_high = systolic_thresholds[age][1];

    // Determine if systolic blood pressure is abnormal or critical
    bool is_abnormal = (systolic < normal_low || systolic > normal_high);
    bool is_critical = (systolic < normal_low - 20 || systolic > normal_high + 20);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "Systolic BP", systolic, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

/**
 * @brief Evaluate diastolic blood pressure against clinical thresholds.
 *
 * @param diastolic Diastolic pressure in mmHg.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_diastolic(float diastolic, vc_age_group_t age) {
    // Define age-specific thresholds based on WHO Emergency Triage guidelines
    static const float diastolic_thresholds[][2] = {
        {40, 70}, // Infant (0-1 year)
        {50, 80}, // Child (1-12 years)
        {60, 90}, // Adult (13-64 years)
        {50, 80}  // Elderly (65+ years)
    };

    // Check for invalid age group
    if (age < 0 || age >= sizeof(diastolic_thresholds) / sizeof(diastolic_thresholds[0])) {
        // Return error result
        vc_threshold_result_t result = {NULL, 0.0f, 0.0f, 0.0f, true, true, "Invalid age group"};
        return result;
    }

    // Evaluate diastolic blood pressure against thresholds
    float normal_low = diastolic_thresholds[age][0];
    float normal_high = diastolic_thresholds[age][1];

    // Determine if diastolic blood pressure is abnormal or critical
    bool is_abnormal = (diastolic < normal_low || diastolic > normal_high);
    bool is_critical = (diastolic < normal_low - 20 || diastolic > normal_high + 20);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "Diastolic BP", diastolic, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

/**
 * @brief Evaluate body temperature against clinical thresholds.
 *
 * @param temperature Temperature in °C.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_temperature(float temperature) {
    // Define temperature thresholds based on WHO Emergency Triage guidelines
    static const float temperature_thresholds[][2] = {
        {36.0f, 37.5f}, // Normal range
        {38.0f, 40.0f}  // Fever range
    };

    // Evaluate temperature against thresholds
    float normal_low = temperature_thresholds[0][0];
    float normal_high = temperature_thresholds[0][1];

    // Determine if temperature is abnormal or critical
    bool is_abnormal = (temperature < normal_low || temperature > normal_high);
    bool is_critical = (temperature < normal_low - 1.0f || temperature > normal_high + 1.0f);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "Body Temperature", temperature, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

/**
 * @brief Evaluate respiratory rate against clinical thresholds.
 *
 * @param rr Respiratory rate in breaths/min.
 * @param age Age group.
 * @return Threshold evaluation result.
 */
vc_threshold_result_t vc_eval_respiratory_rate(float rr, vc_age_group_t age) {
    // Define age-specific thresholds based on WHO Emergency Triage guidelines
    static const float rr_thresholds[][2] = {
        {30, 60}, // Infant (0-1 year)
        {20, 40}, // Child (1-12 years)
        {12, 24}, // Adult (13-64 years)
        {15, 30}  // Elderly (65+ years)
    };

    // Check for invalid age group
    if (age < 0 || age >= sizeof(rr_thresholds) / sizeof(rr_thresholds[0])) {
        // Return error result
        vc_threshold_result_t result = {NULL, 0.0f, 0.0f, 0.0f, true, true, "Invalid age group"};
        return result;
    }

    // Evaluate respiratory rate against thresholds
    float normal_low = rr_thresholds[age][0];
    float normal_high = rr_thresholds[age][1];

    // Determine if respiratory rate is abnormal or critical
    bool is_abnormal = (rr < normal_low || rr > normal_high);
    bool is_critical = (rr < normal_low - 10 || rr > normal_high + 10);

    // Create threshold evaluation result
    vc_threshold_result_t result = {
        "Respiratory Rate", rr, normal_low, normal_high, is_abnormal, is_critical,
        is_abnormal ? "Abnormal" : (is_critical ? "Critical" : "Normal")
    };

    return result;
}

#ifdef __cplusplus
}
#endif