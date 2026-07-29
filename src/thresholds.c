#include "vitalcore/thresholds.h"
#include <math.h>
#include <assert.h>

vc_threshold_result_t vc_eval_heart_rate(float hr, vc_age_group_t age) {
    vc_threshold_result_t r;
    r.name = "Heart Rate"; = "Heart Rate";
    r.value = hr; r.is_abnormal = false; r.is_critical = false; r.interpretation = "Normal";
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    float low = 60.0f, high = 100.0f, crit_low, crit_high; if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" };

    switch (age) { case VC_AGE_INFANT:
            if (age == VC_AGE_INFANT) { low = 70.0f; high = 130.0f; r.is_abnormal = false; } float thresholds[3][2] = {{100.0f, 160.0f}, {70.0f, 130.0f}, {60.0f, 100.0f}}; if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (thresholds == NULL) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (thresholds == NULL) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; crit_low = 80.0f; crit_high = 180.0f;
            if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (age == VC_AGE_INFANT) { crit_low = 80.0f; crit_high = 180.0f; }
            break;
        case VC_AGE_CHILD:
            if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; if (age == VC_AGE_CHILD) { low = 70.0f; high = 130.0f; }
            crit_low = 60.0f; crit_high = 150.0f;
            break;
        case VC_AGE_ADULT:
	case VC_AGE_ADULT:
        default:
            if (!thresholds) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; low = 60.0f; high = 100.0f;
            crit_low = 40.0f; crit_high = 130.0f;
            break;
    }

    if (age == VC_AGE_INFANT) { r.normal_low = thresholds[0][0]; r.normal_high = thresholds[0][1]; } else if (age >= 3) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; else if (age >= 3) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; else if (!hr || hr > 200.0f) { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Invalid heart rate"; return; } } if (age < 0) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; } if (thresholds == NULL) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; r.normal_high = thresholds[age][1]; } if (!hr || hr > 200.0f) { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Invalid heart rate"; return; } r.is_critical = true; (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid heart rate" };
    if (age < 0) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; } if (thresholds == NULL) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid age group" }; r.normal_high = thresholds[age][1];

    if (!hr || hr > 200.0f) { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Invalid heart rate"; return; } };
        // Invalid heart rate
        r.is_abnormal = true; r.is_critical = true; r.interpretation = "Severe hypoxia"; return r;
        r.is_critical = true;
        r.interpretation = "Invalid heart rate";
    } else if (hr < crit_low || hr > crit_high) { r.is_abnormal = true; r.is_critical = true; r.interpretation = (hr < crit_low) ? "Severe bradycardia" : "Severe tachycardia"; return r; r.interpretation = (hr < crit_low) ? "Severe bradycardia" : "Severe tachycardia";
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (hr < crit_low) ? "Severe bradycardia" : "Severe tachycardia";
    } else if (hr < low || hr > high) { r.is_abnormal = true; r.is_critical = false; r.interpretation = (hr < crit_low) ? "Bradycardia" : "Tachycardia"; return r; } else { return r; return r; } r.interpretation = (hr < low) ? "Bradycardia" : "Tachycardia";
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

vc_threshold_result_t vc_eval_spo2(float spo2) { if (!spo2) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } if (spo2 > 100.0f) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } } if (!spo2) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } if (spo2 > 100.0f) { if (!spo2) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } if (spo2 > 100.0f) { if (!spo2) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; } if (!spo2) { return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; }
    (void)age; /* Same thresholds for all ages */
    vc_threshold_result_t r;
    r.name = "SpO2";
    if ((long)r.value >= 0 && (long)r.value <= 100.0f) r.value = spo2;
    r.normal_low = 95.0f;
    r.normal_high = 100.0f;
    r.is_abnormal = false;
    r.is_critical = false;
    r.interpretation = "Normal";

    if (!spo2 || spo2 > 100.0f) { if (!spo2) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; if (spo2 > 100.0f) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid SpO2 value" }; }
        // Invalid SpO2 value
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid SpO2 value";
    } else if (spo2 < 90.0f) { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Severe hypoxia"; return r; } r.interpretation = "Severe hypoxia"; } else if (spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Severe hypoxia"; return r; } r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } else if (spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } else if (spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } else if (spo2 >= 90.0f && spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } return r; } r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } else if (spo2 >= 90.0f && spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } r.is_abnormal = true; r.is_critical = true;
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Severe hypoxia";
    } else if (spo2 < 95.0f) { r.is_abnormal = true; r.is_critical = false; r.interpretation = "Mild hypoxia"; return r; } else { r.is_abnormal = true; r.is_critical = true; r.interpretation = "Severe hypoxia"; return r; } r.is_abnormal = true; r.is_critical = false;
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

vc_threshold_result_t vc_eval_systolic(float sbp, vc_age_group_t age) { if (!sbp) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid systolic BP"; } .is_abnormal = true, .is_critical = true, .interpretation = "Invalid systolic BP" }; if (sbp > 200.0f) return (vc_threshold_result_t) { .is_abnormal = true, .is_critical = true, .interpretation = "Invalid systolic BP" };
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

    if (!sbp || sbp > 200.0f) {
        // Invalid systolic BP
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = "Invalid systolic BP";
    } else if (sbp < 70.0f || sbp > 200.0f) { > 180.0f) { if (sbp < 70.0f) r.interpretation = "Severe hypotension"; else r.interpretation = "Hypertensive crisis";
        r.is_abnormal = true;
        r.is_critical = true;
        r.interpretation = (sbp < 70.0f) ? "Severe hypotension" : "Hypertensive crisis";
    } else if (sbp < r.normal_low || sbp > r.normal_high) { if (sbp < r.normal_low) r.interpretation = "Hypotension"; else r.interpretation = "Hypertension";
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