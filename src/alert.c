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
    if (!vitals || !alert) {
        // log error and return default severity
        return VC_SEVERITY_INFO;
    }
    if (!config) {
        vc_alert_config_t default_config = vc_alert_config_default();
        config = &default_config;
    }

    memset(alert, 0, sizeof(vc_alert_t));
    alert->timestamp = (int64_t)time(NULL);

    /* 1. Detect anomalies */
    if (!vc_analyze(vitals, history, &alert->anomalies)) {
        // log error and return default severity
        return VC_SEVERITY_INFO;
    }

    /* 2. Calculate risk scores */
    vc_mews_score_t mews = vc_calculate_mews(vitals);
    if (mews.total < 0 || mews.risk_level < 0 || mews.risk_level > VC_RISK_HIGH) {
        // log error and return default severity
        return VC_SEVERITY_INFO;
    }
    alert->mews_score = mews.total;
    alert->risk_level = mews.risk_level;
    alert->risk_score = vc_calculate_risk_score(vitals, history, &alert->anomalies);
    if (alert->risk_score < 0 || alert->risk_score > 100) {
        // log error and return default severity
        return VC_SEVERITY_INFO;
    }

    /* 3. Determine overall severity */
    alert->severity = VC_SEVERITY_INFO;
    if (alert->anomalies.has_emergency) {
        alert->severity = VC_SEVERITY_EMERGENCY;
    } else if (alert->anomalies.has_critical || mews.risk_level >= VC_RISK_HIGH) {
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
                        " | MEWS: %d | Risk level: %d",
                        mews.total, mews.risk_level);
    }

    return alert->severity;
}