#include "vitalcore/thresholds.h"
#include <math.h>
#include <assert.h>

vc_threshold_result_t vc_eval_heart_rate(float hr, vc_age_group_t age) {
    vc_threshold_result_t r;
    r.name = "Heart Rate";
    r.value = hr;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    float low, high, crit_low, crit_high;

    switch (age) {
        case VC_AGE_INFANT:
            low = 100.0f; high = 160.0f;
            crit_low = 80.0f; crit_high = 180.0f;
            break;
        case VC_AGE_CHILD:
            low = 70.0f; high = 130.0f;
            crit_low = 60.0f; crit_high = 150.0f;
            break;
        case VC_AGE_ADULT:
        default:
            low = 60.0f; high = 100.0f;
            crit_low = 40.0f; crit_high = 130.0f;
            break;
    }

    r.normal_low = low;
    r.normal_high = high;

    if (hr <= 0.0f || hr > 200.0f) {
        // Invalid heart rate
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid heart rate";
    } else if (hr < crit_low || hr > crit_high) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (hr < crit_low) ? "Severe bradycardia" : "Severe tachycardia";
    } else if (hr < low || hr > high) {
        r.is_abnormal = true;
        r.is_critical = false;
        r.interpretation = (hr < low) ? "Bradycardia" : "Tachycardia";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}

vc_threshold_result_t vc_eval_spo2(float spo2, vc_age_group_t age) {
    (void)age; /* Same thresholds for all ages */
    vc_threshold_result_t r;
    r.name = "SpO2";
    r.value = spo2;
    r.normal_low = 95.0f;
    r.normal_high = 100.0f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    if (spo2 <= 0.0f || spo2 > 100.0f) {
        // Invalid SpO2 value
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid SpO2 value";
    } else if (spo2 < 90.0f) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Severe hypoxia";
    } else if (spo2 < 95.0f) {
        r.is_abnormal = true;
        r.is_critical = false;
        r.interpretation = "Mild hypoxia";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}

vc_threshold_result_t vc_eval_systolic(float sbp, vc_age_group_t age) {
    vc_threshold_result_t r;
    r.name = "Systolic BP";
    r.value = sbp;
    r.normal_low = 90.0f;
    r.normal_high = 140.0f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    switch (age) {
        case VC_AGE_INFANT:
            r.normal_low = 70.0f; r.normal_high = 100.0f;
            break;
        case VC_AGE_CHILD:
            r.normal_low = 80.0f; r.normal_high = 120.0f;
            break;
        case VC_AGE_ADULT:
        default:
            break;
    }

    if (sbp <= 0.0f || sbp > 200.0f) {
        // Invalid systolic BP
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid systolic BP";
    } else if (sbp < 70.0f || sbp > 180.0f) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (sbp < 70.0f) ? "Severe hypotension" : "Hypertensive crisis";
    } else if (sbp < r.normal_low || sbp > r.normal_high) {
        r.is_abnormal = true;
        r.is_critical = false;
        r.interpretation = (sbp < r.normal_low) ? "Hypotension" : "Hypertension";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}

vc_threshold_result_t vc_eval_diastolic(float dbp, vc_age_group_t age) {
    vc_threshold_result_t r;
    r.name = "Diastolic BP";
    r.value = dbp;
    r.normal_low = 60.0f;
    r.normal_high = 90.0f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    switch (age) {
        case VC_AGE_INFANT:
            r.normal_low = 45.0f; r.normal_high = 65.0f;
            break;
        case VC_AGE_CHILD:
            r.normal_low = 50.0f; r.normal_high = 80.0f;
            break;
        case VC_AGE_ADULT:
        default:
            break;
    }

    if (dbp <= 0.0f || dbp > 120.0f) {
        // Invalid diastolic BP
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid diastolic BP";
    } else if (dbp < 40.0f || dbp > 120.0f) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (dbp < 40.0f) ? "Severe hypotension" : "Hypertensive crisis";
    } else if (dbp < r.normal_low || dbp > r.normal_high) {
        r.is_abnormal = true;
        r.is_critical = false;
        r.interpretation = (dbp < r.normal_low) ? "Low diastolic" : "High diastolic";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}

vc_threshold_result_t vc_eval_temperature(float temp) {
    vc_threshold_result_t r;
    r.name = "Temperature";
    r.value = temp;
    r.normal_low = 36.1f;
    r.normal_high = 37.2f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    if (temp <= 0.0f || temp > 50.0f) {
        // Invalid temperature
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid temperature";
    } else if (temp < 32.0f || temp > 41.0f) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (temp < 32.0f) ? "Severe hypothermia" : "Hyperpyrexia";
    } else if (temp < 35.0f || temp > 38.3f) {
        r.is_abnormal = true;
        r.is_critical = (temp < 34.0f || temp > 40.0f);
        r.interpretation = (temp < 35.0f) ? "Hypothermia" : "Fever";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}

vc_threshold_result_t vc_eval_respiratory_rate(float rr, vc_age_group_t age) {
    vc_threshold_result_t r;
    r.name = "Respiratory Rate";
    r.value = rr;
    r.normal_low = 12.0f;
    r.normal_high = 20.0f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    switch (age) {
        case VC_AGE_INFANT:
            r.normal_low = 30.0f; r.normal_high = 60.0f;
            break;
        case VC_AGE_CHILD:
            r.normal_low = 18.0f; r.normal_high = 30.0f;
            break;
        case VC_AGE_ADULT:
        default:
            break;
    }

    if (rr <= 0.0f || rr > 60.0f) {
        // Invalid respiratory rate
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid respiratory rate";
    } else if (rr < 8.0f || rr > 40.0f) {
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (rr < 8.0f) ? "Severe bradypnea" : "Severe tachypnea";
    } else if (rr < r.normal_low || rr > r.normal_high) {
        r.is_abnormal = true;
        r.is_critical = false;
        r.interpretation = (rr < r.normal_low) ? "Bradypnea" : "Tachypnea";
    } else {
        r.is_abnormal = false;
        r.is_critical = false;
        r.interpretation = "Normal";
    }

    return r;
}