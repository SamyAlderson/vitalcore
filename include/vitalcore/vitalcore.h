/**
 * @file vitalcore.h
 * @brief Main API for VitalCore — real-time vital signs monitoring.
 *
 * This is the primary header to include. It provides the high-level
 * monitor API for analyzing patient vital signs and generating alerts.
 */

#ifndef VITALCORE_H
#define VITALCORE_H

#include "vitals.h"
#include "thresholds.h"
#include "anomaly.h"
#include "risk.h"
#include "alert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Version information.
 */
#define VITALCORE_VERSION_MAJOR 1
#define VITALCORE_VERSION_MINOR 0
#define VITALCORE_VERSION_PATCH 0
#define VITALCORE_VERSION_STRING "1.0.0"

/**
 * @brief VitalCore monitor context.
 *
 * Opaque structure holding monitor state, history, and configuration.
 * Created with vc_monitor_create(), destroyed with vc_monitor_destroy().
 */
typedef struct vc_monitor vc_monitor_t;

/**
 * @brief Create a new VitalCore monitor.
 *
 * @return Pointer to new monitor, or NULL on failure.
 */
vc_monitor_t *vc_monitor_create(void);

/**
 * @brief Create a monitor with custom configuration.
 *
 * @param history_capacity Number of historical readings to retain.
 * @param trend_window Time window for trend analysis in minutes.
 * @return Pointer to new monitor, or NULL on failure.
 */
vc_monitor_t *vc_monitor_create_custom(uint32_t history_capacity,
                                       uint32_t trend_window);

/**
 * @brief Destroy a monitor and free all resources.
 *
 * @param monitor Pointer to monitor to destroy.
 */
void vc_monitor_destroy(vc_monitor_t *monitor);

/**
 * @brief Analyze vital signs and generate a clinical alert.
 *
 * This is the main entry point. It:
 * 1. Adds the reading to history
 * 2. Evaluates against clinical thresholds
 * 3. Detects anomalies
 * 4. Calculates risk score (MEWS + composite)
 * 5. Generates a clinical alert
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to analyze.
 * @param alert Output: generated clinical alert.
 * @return Severity of the generated alert.
 */
vc_severity_t vc_monitor_analyze(vc_monitor_t *monitor,
                                 const vc_vitals_t *vitals,
                                 vc_alert_t *alert);

/**
 * @brief Set the alert configuration for a monitor.
 *
 * @param monitor Monitor context.
 * @param config New alert configuration.
 */
void vc_monitor_set_alert_config(vc_monitor_t *monitor,
                                 const vc_alert_config_t *config);

/**
 * @brief Get the current risk score from the monitor.
 *
 * @param monitor Monitor context.
 * @return Current composite risk score (0-100).
 */
float vc_monitor_get_risk_score(const vc_monitor_t *monitor);

/**
 * @brief Get the current MEWS score from the monitor.
 *
 * @param monitor Monitor context.
 * @return Current MEWS total score.
 */
int vc_monitor_get_mews_score(const vc_monitor_t *monitor);

/**
 * @brief Get the vitals history from the monitor.
 *
 * @param monitor Monitor context.
 * @return Pointer to vitals history (owned by monitor, do not free).
 */
const vc_vitals_history_t *vc_monitor_get_history(const vc_monitor_t *monitor);

/**
 * @brief Get library version string.
 *
 * @return Version string (e.g., "1.0.0").
 */
const char *vc_version(void);

#ifdef __cplusplus
}
#endif

#endif /* VITALCORE_H */
