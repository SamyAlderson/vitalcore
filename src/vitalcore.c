/**
 * @file vitalcore.c
 * @brief High-level monitor API implementation.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vitalcore/vitalcore.h"

#define DEFAULT_HISTORY_CAPACITY 64
#define DEFAULT_TREND_WINDOW 30

struct vc_monitor {
    vc_vitals_history_t *history;
    vc_alert_config_t alert_config;
    float last_risk_score;
    int last_mews_score;
};

vc_monitor_t *vc_monitor_create(void) {
    return vc_monitor_create_custom(DEFAULT_HISTORY_CAPACITY, DEFAULT_TREND_WINDOW);
}

vc_monitor_t *vc_monitor_create_custom(uint32_t history_capacity,
                                       uint32_t trend_window) {
    vc_monitor_t *m = (vc_monitor_t *)calloc(1, sizeof(vc_monitor_t));
    if (!m) return NULL;

    m->history = vc_vitals_history_create(history_capacity, trend_window);
    if (!m->history) {
        free(m);
        return NULL;
    }

    m->alert_config = vc_alert_config_default();
    m->last_risk_score = 0.0f;
    m->last_mews_score = 0;

    return m;
}

void vc_monitor_destroy(vc_monitor_t *monitor) {
    if (!monitor) return;
    vc_vitals_history_destroy(monitor->history);
    free(monitor);
}

vc_severity_t vc_monitor_analyze(vc_monitor_t *monitor,
                                 const vc_vitals_t *vitals,
                                 vc_alert_t *alert) {
    if (!monitor || !vitals || !alert) return VC_SEVERITY_INFO;

    /* Add to history */
    vc_vitals_t reading = *vitals;
    if (reading.timestamp == 0) {
        reading.timestamp = (int64_t)time(NULL);
    }
    if (monitor->history) {
        vc_vitals_history_add(monitor->history, &reading);
    }

    /* Auto-calculate MAP if not provided */
    if (reading.mean_arterial == 0.0f &&
        vc_vitals_is_present(reading.systolic) &&
        vc_vitals_is_present(reading.diastolic)) {
        if (reading.systolic != 0.0f && reading.diastolic != 0.0f) {
            reading.mean_arterial = vc_calculate_map(reading.systolic, reading.diastolic);
        }
    }

    /* Generate alert */
    if (monitor->history) {
        vc_severity_t severity = vc_generate_alert(vitals, monitor->history,
                                                    &monitor->alert_config, alert);
        return severity;
    }

    return VC_SEVERITY_INFO;
}

void vc_monitor_set_alert_config(vc_monitor_t *monitor,
                                 const vc_alert_config_t *config) {
    if (monitor && config) {
        monitor->alert_config = *config;
    }
}

float vc_monitor_get_risk_score(const vc_monitor_t *monitor) {
    return monitor ? monitor->last_risk_score : 0.0f;
}

int vc_monitor_get_mews_score(const vc_monitor_t *monitor) {
    return monitor ? monitor->last_mews_score : 0;
}

const vc_vitals_history_t *vc_monitor_get_history(const vc_monitor_t *monitor) {
    return monitor ? monitor->history : NULL;
}

const char *vc_version(void) {
    return VITALCORE_VERSION_STRING;
}