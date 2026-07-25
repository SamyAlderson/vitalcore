/**
 * @file alert.c
 * @brief Clinical alert engine implementation.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "vitalcore/alert.h"
#include "vitalcore/anomaly.h"
#include "vitalcore/risk.h"

vc_alert_config_t vc_alert_config_default(void) {
    vc_alert_config_t config;
    config.format = VC_FORMAT_TEXT;
    config.min_severity = VC_SEVERITY_INFO;
    config.include_details = true;
    config.include_recommendation = true;
    config.enable_trend_alerts = true;
    config.trend_window_minutes = 30;
    return config;
}

vc_severity_t vc_generate_alert(const vc_vitals_t *vitals,
                                const vc_vitals_history_t *history,
                                const vc_alert_config_t *config,
                                vc_alert_t *alert) {
    if (!vitals || !alert) return VC_SEVERITY_INFO;
    if (!config) {
        vc_alert_config_t default_config = vc_alert_config_default();
        config = &default_config;
    }

    memset(alert, 0, sizeof(vc_alert_t));
    alert->timestamp = (int64_t)time(NULL);

    /* 1. Detect anomalies */
    if (!vc_analyze(vitals, history, &alert->anomalies)) {
        // Handle analyze failure
        alert->severity = VC_SEVERITY_INFO;
        return alert->severity;
    }

    /* 2. Calculate risk scores */
    vc_mews_score_t mews = vc_calculate_mews(vitals);
    if (mews.total == 0) {
        // Handle invalid MEWS score
        alert->mews_score = 0;
        alert->risk_level = VC_RISK_LOW;
        alert->risk_score = 0.0;
    } else {
        alert->mews_score = mews.total;
        alert->risk_level = mews.risk_level;
        alert->risk_score = vc_calculate_risk_score(vitals, history, &alert->anomalies);
    }

    /* 3. Determine overall severity */
    alert->severity = VC_SEVERITY_INFO;
    if (alert->anomalies.has_emergency) {
        alert->severity = VC_SEVERITY_EMERGENCY;
    } else if (alert->anomalies.count > 0 || (mews.risk_level >= VC_RISK_HIGH && mews.total > 0)) {
        alert->severity = VC_SEVERITY_CRITICAL;
    } else if (alert->anomalies.count > 0 || mews.risk_level >= VC_RISK_MODERATE) {
        alert->severity = VC_SEVERITY_WARNING;
    }

    /* 4. Build message */
    if (alert->anomalies.count == 0) {
        snprintf(alert->message, sizeof(alert->message),
                 "All vital signs within normal limits");
    } else {
        int offset = 0;
        for (uint32_t i = 0; i < alert->anomalies.count && offset < 500; i++) {
            const vc_anomaly_t *a = &alert->anomalies.anomalies[i];
            offset += snprintf(alert->message + offset, sizeof(alert->message) - offset,
                               "%s[%s] %s: %.1f (threshold: %.1f)%s",
                               (i > 0) ? " | " : "",
                               vc_severity_name(a->severity),
                               a->name, a->value, a->threshold,
                               (i < alert->anomalies.count - 1) ? "" : "");
        }
    }

    /* 5. Build details */
    if (config->include_details) {
        int off = 0;
        off += snprintf(alert->details + off, sizeof(alert->details) - off,
                        "HR: %.0f bpm | SpO2: %.0f%% | BP: %.0f/%.0f mmHg | "
                        "Temp: %.1f°C | RR: %.0f/min",
                        vitals->heart_rate, vitals->spo2,
                        vitals->systolic, vitals->diastolic,
                        vitals->temperature, vitals->respiratory_rate);
        off += snprintf(alert->details + off, sizeof(alert->details) - off,
                        " | MEWS: %d | Risk: %.1f%% | Level: %s",
                        alert->mews_score, alert->risk_score,
                        vc_risk_level_name(alert->risk_level));
    }

    /* 6. Build recommendation */
    if (config->include_recommendation) {
        snprintf(alert->recommendation, sizeof(alert->recommendation),
                 "%s", vc_risk_level_action(alert->risk_level));
    }

    return alert->severity;
}

uint32_t vc_alert_format_text(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size) {
    if (!alert || !buffer || buffer_size == 0) return 0;

    int off = 0;
    off += snprintf(buffer + off, buffer_size - off,
                    "=== VITALCORE ALERT [%s] ===\n", vc_severity_name(alert->severity));
    off += snprintf(buffer + off, buffer_size - off,
                    "Risk Level: %s (Score: %.1f%%)\n", vc_risk_level_name(alert->risk_level),
                    alert->risk_score);
    off += snprintf(buffer + off, buffer_size - off,
                    "MEWS Score: %d/14\n", alert->mews_score);
    off += snprintf(buffer + off, buffer_size - off,
                    "Message: %s\n", alert->message);
    if (alert->details[0]) {
        off += snprintf(buffer + off, buffer_size - off,
                        "Details: %s\n", alert->details);
    }
    if (alert->recommendation[0]) {
        off += snprintf(buffer + off, buffer_size - off,
                        "Action: %s\n", alert->recommendation);
    }
    off += snprintf(buffer + off, buffer_size - off,
                    "Anomalies: %u detected\n", alert->anomalies.count);
    off += snprintf(buffer + off, buffer_size - off,
                    "========================\n");

    return (uint32_t)off;
}

uint32_t vc_alert_format_json(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size) {
    if (!alert || !buffer || buffer_size == 0) return 0;

    int off = 0;
    off += snprintf(buffer + off, buffer_size - off,
                    "{\"severity\":\"%s\",\"risk_level\":\"%s\","
                    "\"risk_score\":%.1f,\"mews_score\":%d,"
                    "\"anomaly_count\":%u,\"message\":\"",
                    vc_severity_name(alert->severity),
                    vc_risk_level_name(alert->risk_level),
                    alert->risk_score, alert->mews_score,
                    alert->anomalies.count);

    /* Escape message for JSON */
    for (const char *p = alert->message; *p && (uint32_t)off < buffer_size - 20; p++) {
        if (*p == '"') buffer[off++] = '\\';
        buffer[off++] = *p;
    }

    off += snprintf(buffer + off, buffer_size - off,
                    "\",\"recommendation\":\"%s\"}",
                    alert->recommendation);

    return (uint32_t)off;
}