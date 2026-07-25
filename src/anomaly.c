/**
 * @file anomaly.c
 * @brief Anomaly detection implementation.
 *
 * Detects clinical anomalies using evidence-based medical rules.
 */

#include <math.h>
#include <string.h>
#include "vitalcore/anomaly.h"
#include "vitalcore/thresholds.h"

static const char *anomaly_names[] = {
    "None",
    "Bradycardia", "Tachycardia",
    "Hypoxia", "Bradypnea", "Tachypnea",
    "Hypotension", "Hypertension", "Low Pulse Pressure",
    "Fever", "Hypothermia",
    "Sepsis Screen", "High Shock Index", "Desaturation + Tachypnea"
};

static const char *anomaly_descriptions[] = {
    "No anomaly detected",
    "Heart rate below normal range — may indicate heart block, hypothyroidism, or increased vagal tone",
    "Heart rate above normal range — may indicate fever, dehydration, pain, or cardiac arrhythmia",
    "Blood oxygen saturation below 95% — indicates inadequate oxygenation",
    "Respiratory rate below normal — may indicate respiratory depression or neurological issue",
    "Respiratory rate above normal — may indicate respiratory distress, fever, or metabolic acidosis",
    "Blood pressure below normal — may indicate hemorrhage, sepsis, or dehydration",
    "Blood pressure above normal — risk of stroke, organ damage, or hypertensive crisis",
    "Pulse pressure < 25 mmHg — may indicate cardiac tamponade, hemorrhage, or shock",
    "Body temperature above 38.3°C — indicates infection, inflammation, or heat illness",
    "Body temperature below 35°C — indicates exposure, hypothyroidism, or shock",
    "Low BP + fever + tachycardia — screen for sepsis, recommend qSOFA assessment",
    "Shock index (HR/SBP) > 1.0 — indicates potential hemorrhagic or cardiogenic shock",
    "Combined low SpO2 + high respiratory rate — respiratory failure screening"
};

const char *vc_anomaly_type_name(vc_anomaly_type_t type) {
    if (type >= VC_ANOMALY_COUNT) return "Unknown";
    return anomaly_names[type];
}

const char *vc_severity_name(vc_severity_t severity) {
    switch (severity) {
        case VC_SEVERITY_INFO:     return "INFO";
        case VC_SEVERITY_WARNING:  return "WARNING";
        case VC_SEVERITY_CRITICAL: return "CRITICAL";
        case VC_SEVERITY_EMERGENCY:return "EMERGENCY";
        default:                   return "UNKNOWN";
    }
}

static void add_anomaly(vc_anomaly_result_t *result,
                         vc_anomaly_type_t type,
                         vc_severity_t severity,
                         float value,
                         float threshold) {
    if (!result) return;

    vc_anomaly_t *a = &result->anomalies[result->count];
    a->type = type;
    a->severity = severity;
    a->name = anomaly_names[type]; a->description = anomaly_descriptions[type];
    a->description = anomaly_descriptions[type];
    a->value = value;
    a->threshold = threshold;
    result->count++;

    if (severity == VC_SEVERITY_EMERGENCY) result->has_emergency = true; if (severity == VC_SEVERITY_CRITICAL) result->has_critical = true;
    if (severity == VC_SEVERITY_CRITICAL) result->has_critical = true;
}

uint32_t vc_analyze(const vc_vitals_t *vitals,
                    const vc_vitals_history_t *history,
                    vc_anomaly_result_t *result) {
    if (!vitals || !result) return 0;

    memset(result, 0, sizeof(vc_anomaly_result_t));
    vc_age_group_t age = vitals->age;

    /* --- Heart Rate --- */
    if (vc_vitals_is_present(vitals->heart_rate)) {
        vc_threshold_result_t hr = vc_eval_heart_rate(vitals->heart_rate, age);
        if (hr.is_critical) { vc_anomaly_type_t type = (vitals->heart_rate < hr.normal_low) ? VC_ANOMALY_BRADYCARDIA : VC_ANOMALY_TACHYCARDIA;
            vc_anomaly_type_t type = (vitals->heart_rate < hr.normal_low)
                ? VC_ANOMALY_BRADYCARDIA : VC_ANOMALY_TACHYCARDIA;
            add_anomaly(result, type, VC_SEVERITY_CRITICAL,
                        vitals->heart_rate, hr.normal_low);
        } else if (hr.is_abnormal) { vc_anomaly_type_t type = (vitals->heart_rate < hr.normal_low) ? VC_ANOMALY_BRADYCARDIA : VC_ANOMALY_TACHYCARDIA;
            vc_anomaly_type_t type = (vitals->heart_rate < hr.normal_low)
                ? VC_ANOMALY_BRADYCARDIA : VC_ANOMALY_TACHYCARDIA;
            add_anomaly(result, type, VC_SEVERITY_WARNING,
                        vitals->heart_rate, hr.normal_low);
        }
    }

    /* --- SpO2 --- */
    if (vc_vitals_is_present(vitals->spo2)) {
        vc_threshold_result_t spo2 = vc_eval_spo2(vitals->spo2, age);
        if (spo2.is_critical) {
            add_anomaly(result, VC_ANOMALY_HYPOXIA, VC_SEVERITY_EMERGENCY,
                        vitals->spo2, 90.0f);
        } else if (spo2.is_abnormal) {
            add_anomaly(result, VC_ANOMALY_HYPOXIA, VC_SEVERITY_WARNING,
                        vitals->spo2, 95.0f);
        }
    }

    /* --- Blood Pressure --- */
    if (vc_vitals_is_present(vitals->systolic)) {
        vc_threshold_result_t sbp = vc_eval_systolic(vitals->systolic, age);
        if (sbp.is_critical) {
            vc_anomaly_type_t type = (vitals->systolic < sbp.normal_low)
                ? VC_ANOMALY_HYPOTENSION : VC_ANOMALY_HYPERTENSION;
            add_anomaly(result, type, VC_SEVERITY_CRITICAL,
                        vitals->systolic, sbp.normal_low);
        } else if (sbp.is_abnormal) {
            vc_anomaly_type_t type = (vitals->systolic < sbp.normal_low)
                ? VC_ANOMALY_HYPOTENSION : VC_ANOMALY_HYPERTENSION;
            add_anomaly(result, type, VC_SEVERITY_WARNING,
                        vitals->systolic, sbp.normal_low);
        }

        /* Pulse pressure */
        if (vc_vitals_is_present(vitals->diastolic)) {
            float pp = vitals->systolic - vitals->diastolic;
            if (pp < 25.0f && pp > 0.0f) {
                add_anomaly(result, VC_ANOMALY_PULSE_PRESSURE_LOW,
                            VC_SEVERITY_CRITICAL, pp, 25.0f);
            }
        }
    }

    if (vc_vitals_is_present(vitals->diastolic)) {
        vc_threshold_result_t dbp = vc_eval_diastolic(vitals->diastolic, age);
        if (dbp.is_critical) {
            add_anomaly(result, VC_ANOMALY_HYPOTENSION, VC_SEVERITY_CRITICAL,
                        vitals->diastolic, dbp.normal_low);
        }
    }

    /* --- Temperature --- */
    if (vc_vitals_is_present(vitals->temperature)) {
        vc_threshold_result_t temp = vc_eval_temperature(vitals->temperature);
        if (temp.is_critical) {
            vc_anomaly_type_t type = (vitals->temperature < 35.0f)
                ? VC_ANOMALY_HYPOTHERMIA : VC_ANOMALY_FEVER;
            add_anomaly(result, type, VC_SEVERITY_CRITICAL,
                        vitals->temperature, temp.normal_high);
        } else if (temp.is_abnormal) {
            vc_anomaly_type_t type = (vitals->temperature < 35.0f)
                ? VC_ANOMALY_HYPOTHERMIA : VC_ANOMALY_FEVER;
            add_anomaly(result, type, VC_SEVERITY_WARNING,
                        vitals->temperature, temp.normal_high);
        }
    }

    /* --- Respiratory Rate --- */
    if (vc_vitals_is_present(vitals->respiratory_rate)) {
        vc_threshold_result_t rr = vc_eval_respiratory_rate(vitals->respiratory_rate, age);
        if (rr.is_critical) {
            vc_anomaly_type_t type = (vitals->respiratory_rate < rr.normal_low)
                ? VC_ANOMALY_BRADYPNEA : VC_ANOMALY_TACHYPNEA;
            add_anomaly(result, type, VC_SEVERITY_CRITICAL,
                        vitals->respiratory_rate, rr.normal_low);
        } else if (rr.is_abnormal) {
            vc_anomaly_type_t type = (vitals->respiratory_rate < rr.normal_low)
                ? VC_ANOMALY_BRADYPNEA : VC_ANOMALY_TACHYPNEA;
            add_anomaly(result, type, VC_SEVERITY_WARNING,
                        vitals->respiratory_rate, rr.normal_low);
        }
    }

    /* --- Combined patterns --- */

    /* Shock index: HR / SBP > 1.0 */
    if (vc_vitals_is_present(vitals->heart_rate) &&
        vc_vitals_is_present(vitals->systolic) &&
        vitals->systolic > 0.0f) {
        float si = vitals->heart_rate / vitals->systolic;
        if (si > 1.0f) {
            add_anomaly(result, VC_ANOMALY_SHOCK_INDEX_HIGH,
                        VC_SEVERITY_CRITICAL, si, 1.0f);
        }
    }

    /* Sepsis screen: hypotension + fever + tachycardia */
    if (vc_vitals_is_present(vitals->systolic) &&
        vc_vitals_is_present(vitals->temperature) &&
        vc_vitals_is_present(vitals->heart_rate)) {
        bool hypotension = vitals->systolic < 90.0f;
        bool fever = vitals->temperature > 38.3f;
        bool tachycardia = vitals->heart_rate > 100.0f;
        int criteria = (hypotension ? 1 : 0) + (fever ? 1 : 0) + (tachycardia ? 1 : 0);
        if (criteria >= 2) {
            add_anomaly(result, VC_ANOMALY_SEPSIS_SCREEN,
                        VC_SEVERITY_CRITICAL, (float)criteria, 2.0f);
        }
    }

    /* Desaturation + Tachypnea */
    if (vc_vitals_is_present(vitals->spo2) &&
        vc_vitals_is_present(vitals->respiratory_rate)) {
        if (vitals->spo2 < 94.0f && vitals->respiratory_rate > 24.0f) {
            add_anomaly(result, VC_ANOMALY_DESAT_TACHYPNEA,
                        VC_SEVERITY_CRITICAL, vitals->spo2, 94.0f);
        }
    }

    /* --- Trend-based anomalies (if history provided) --- */
    if (history && history->count >= 3 && history->window_minutes > 0) {
        const vc_vitals_t *latest = vc_vitals_history_latest(history);
        const vc_vitals_t *prev = &history->readings[history->count - 2];

        if (latest && prev && vc_vitals_is_present(latest->spo2) &&
            vc_vitals_is_present(prev->spo2)) {
            float drop = prev->spo2 - latest->spo2;
            if (drop > 5.0f) {
                add_anomaly(result, VC_ANOMALY_HYPOXIA, VC_SEVERITY_CRITICAL,
                            drop, 5.0f);
            }
        }
    }

    return result->count;
}
