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
 * @brief Validate the history capacity and trend window.
 *
 * @param history_capacity Number of historical readings to retain.
 * @param trend_window Time window for trend analysis in minutes.
 * @return 0 on success, -1 on failure.
 */
int vc_validate_history_capacity_and_trend_window(uint32_t history_capacity,
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
 * @brief Add a reading to the monitor history.
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to add to history.
 * @return 0 on success, -1 on failure.
 */
int vc_monitor_add_to_history(vc_monitor_t *monitor,
                              const vc_vitals_t *vitals);

/**
 * @brief Evaluate vital signs against clinical thresholds.
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to evaluate.
 * @return 0 on success, -1 on failure.
 */
int vc_monitor_evaluate_thresholds(vc_monitor_t *monitor,
                                   const vc_vitals_t *vitals);

/**
 * @brief Detect anomalies in vital signs.
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to analyze.
 * @return 0 on success, -1 on failure.
 */
int vc_monitor_detect_anomalies(vc_monitor_t *monitor,
                                 const vc_vitals_t *vitals);

/**
 * @brief Calculate the risk score for vital signs.
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to analyze.
 * @return Composite risk score (0-100).
 */
float vc_monitor_calculate_risk_score(vc_monitor_t *monitor,
                                      const vc_vitals_t *vitals);

/**
 * @brief Generate a clinical alert based on the risk score.
 *
 * @param monitor Monitor context.
 * @param vitals Vital signs to analyze.
 * @param alert Output: generated clinical alert.
 * @return Severity of the generated alert.
 */
vc_severity_t vc_monitor_generate_alert(vc_monitor_t *monitor,
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
 * @brief Validate the alert configuration.
 *
 * @param config New alert configuration.
 * @return 0 on success, -1 on failure.
 */
int vc_validate_alert_config(const vc_alert_config_t *config);

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
const vc_vitals_history_t *vc_monitor_get_vitals_history(const vc_monitor_t *monitor);

/**
 * @brief Validate the monitor context.
 *
 * @param monitor Monitor context.
 * @return 0 on success, -1 on failure.
 */
int vc_validate_monitor_context(vc_monitor_t *monitor);

#ifdef __cplusplus
}
#endif

#endif  // VITALCORE_H