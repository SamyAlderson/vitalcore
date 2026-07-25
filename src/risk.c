/**
 * @file risk.c
 * @brief Risk scoring implementation.
 *
 * Implements MEWS (Modified Early Warning Score) and composite
 * risk scoring for patient deterioration detection.
 */

#include <math.h>
#include <string.h>
#include "vitalcore/risk.h"
#include "vitalcore/thresholds.h"
#include "vitalcore/anomaly.h"

static int mews_heart_rate(float hr) {
    if (hr <= 40.0f) return 3;
    if (hr <= 50.0f) return 2;
    if (hr <= 100.0f) return 0;
    if (hr <= 110.0f) return 1;
    if (hr <= 129.0f) return 2;
    return 3;
}

static int mews_systolic(float sbp) {
    if (sbp <= 70.0f) return 3;
    if (sbp <= 80.0f) return 2;
    if (sbp <= 100.0f) return 1;
    if (sbp <= 199.0f) return 0;
    return 3;
}

static int mews_respiratory_rate(float rr) {
    if (rr <= 8.0f) return 3;
    if (rr <= 14.0f) return 0;
    if (rr <= 20.0f) return 1;
    if (rr <= 29.0f) return 2;
    return 3;
}

static int mews_temperature(float temp) {
    if (temp <= 35.0f) return 2;
    if (temp <= 38.4f) return 0;
    if (temp <= 39.0f) return 1;
    return 2;
}

vc_mews_score_t vc_calculate_mews(const vc_vitals_t *vitals) {
    vc_mews_score_t mews;
    memset(&mews, 0, sizeof(mews));

    if (!vitals || !vitals->heart_rate || !vitals->systolic || !vitals->respiratory_rate || !vitals->temperature) {
        return mews;
    }

    mews.heart_rate_score = mews_heart_rate(vitals->heart_rate);
    mews.systolic_score = mews_systolic(vitals->systolic);
    mews.respiratory_rate_score = mews_respiratory_rate(vitals->respiratory_rate);
    mews.temperature_score = mews_temperature(vitals->temperature);
    mews.consciousness_score = 0; /* AVPU not available by default */

    mews.total = mews.heart_rate_score + mews.systolic_score +
                 mews.respiratory_rate_score + mews.temperature_score +
                 mews.consciousness_score;

    if (mews.total <= 2) {
        mews.risk_level = VC_RISK_LOW;
    } else if (mews.total <= 4) {
        mews.risk_level = VC_RISK_MODERATE;
    } else if (mews.total <= 6) {
        mews.risk_level = VC_RISK_HIGH;
    } else {
        mews.risk_level = VC_RISK_CRITICAL;
    }

    return mews;
}

const char *vc_risk_level_name(vc_risk_level_t level) {
    switch (level) {
        case VC_RISK_LOW:      return "LOW";
        case VC_RISK_MODERATE: return "MODERATE";
        case VC_RISK_HIGH:     return "HIGH";
        case VC_RISK_CRITICAL: return "CRITICAL";
        default:               return "UNKNOWN";
    }
}

const char *vc_risk_level_action(vc_risk_level_t level) {
    switch (level) {
        case VC_RISK_LOW:      return "Routine monitoring";
        case VC_RISK_MODERATE: return "Increase monitoring frequency, inform nurse";
        case VC_RISK_HIGH:     return "Urgent clinical assessment needed";
        case VC_RISK_CRITICAL: return "Immediate medical intervention required";
        default:               return "Unknown";
    }
}

static float compute_deviation_score(const vc_vitals_t *vitals) {
    float score = 0.0f;
    int count = 0;

    if (vitals->heart_rate) {
        float dev = fabsf(vitals->heart_rate - 80.0f) / 80.0f;
        score += dev * 25.0f;
        count++;
    }

    if (vitals->spo2) {
        float dev = fabsf(vitals->spo2 - 98.0f) / 98.0f;
        score += dev * 30.0f;
        count++;
    }

    if (vitals->systolic) {
        float dev = fabsf(vitals->systolic - 120.0f) / 120.0f;
        score += dev * 20.0f;
        count++;
    }

    if (vitals->temperature) {
        float dev = fabsf(vitals->temperature - 37.0f) / 37.0f;
        score += dev * 15.0f;
        count++;
    }

    if (vitals->respiratory_rate) {
        float dev = fabsf(vitals->respiratory_rate - 16.0f) / 16.0f;
        score += dev * 10.0f;
        count++;
    }

    return (count > 0) ? score / (float)count : 0.0f;
}

float vc_calculate_risk_score(const vc_vitals_t *vitals,
                              const vc_vitals_history_t *history,
                              const vc_anomaly_result_t *anomalies) {
    if (!vitals) return 0.0f;

    float score = 0.0f;

    vc_mews_score_t mews = vc_calculate_mews(vitals);
    score += (float)mews.total / 14.0f * 40.0f;

    score += compute_deviation_score(vitals);

    if (anomalies) {
        for (uint32_t i = 0; i < anomalies->count; i++) {
            switch (anomalies->anomalies[i].severity) {
                case VC_SEVERITY_EMERGENCY: score += 12.5f; break;
                case VC_SEVERITY_CRITICAL:  score += 8.0f; break;
                case VC_SEVERITY_WARNING:   score += 3.0f; break;
                default: break;
            }
        }
        if (score > 25.0f) score = 25.0f;
    }

    if (history && history->count >= 2) {
        const vc_vitals_t *latest = vc_vitals_history_latest(history);
        const vc_vitals_t *prev = &history->readings[history->count - 2];
        if (latest && prev) {
            float trend_score = 0.0f;
            if (latest->spo2 && prev->spo2) {
                float drop = prev->spo2 - latest->spo2;
                if (drop > 3.0f) trend_score += 5.0f;
            }
            if (latest->heart_rate && prev->heart_rate) {
                float hr_change = fabsf(latest->heart_rate - prev->heart_rate);
                if (hr_change > 20.0f) trend_score += 3.0f;
            }
            if (latest->systolic && prev->systolic) {
                float bp_drop = prev->systolic - latest->systolic;
                if (bp_drop > 20.0f) trend_score += 5.0f;
            }
            score += (trend_score > 10.0f) ? 10.0f : trend_score;
        }
    }

    if (score < 0.0f) score = 0.0f;
    if (score > 100.0f) score = 100.0f;

    return score;
}