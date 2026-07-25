#ifndef VITALCORE_ALERT_H
#define VITALCORE_ALERT_H

#include "vitals.h"
#include "anomaly.h"
#include "risk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Alert output format.
 */
typedef enum {
    VC_FORMAT_TEXT = 0,      /**< Plain text */
    VC_FORMAT_JSON,          /**< JSON format */
    VC_FORMAT_SILENT         /**< No output, data only */
} vc_alert_format_t;

/**
 * @brief Clinical alert.
 */
typedef struct {
    vc_severity_t severity;                 /**< Alert severity */
    vc_risk_level_t risk_level;             /**< Risk classification */
    float risk_score;                       /**< Composite risk score (0-100) */
    int mews_score;                         /**< MEWS total score */
    char message[512];                      /**< Human-readable alert message */
    char details[1024];                     /**< Detailed breakdown */
    char recommendation[256];               /**< Recommended clinical action */
    vc_anomaly_result_t anomalies;          /**< Underlying anomaly data */
    int64_t timestamp;                      /**< Alert timestamp */
} vc_alert_t;

/**
 * @brief Alert configuration.
 */
typedef struct {
    vc_alert_format_t format;               /**< Output format */
    vc_severity_t min_severity;             /**< Minimum severity to generate alert */
    bool include_details;                   /**< Include detailed breakdown */
    bool include_recommendation;            /**< Include recommended actions */
    bool enable_trend_alerts;               /**< Enable trend-based alerts */
    uint32_t trend_window_minutes;          /**< Time window for trend analysis */
} vc_alert_config_t;

/**
 * @brief Create default alert configuration.
 *
 * @return Default configuration (text format, all severities, full details).
 */
vc_alert_config_t vc_alert_config_default(void);

/**
 * @brief Generate clinical alert from vital signs.
 *
 * @param vitals Current vital signs.
 * @param history Optional history for trend analysis.
 * @param config Alert configuration.
 * @param alert Output: generated alert.
 * @return Severity of the generated alert.
 */
vc_severity_t vc_generate_alert(const vc_vitals_t *vitals,
                                const vc_vitals_history_t *history,
                                const vc_alert_config_t *config,
                                vc_alert_t *alert);

/**
 * @brief Format alert as text string.
 *
 * @param alert Alert to format.
 * @param buffer Output buffer.
 * @param buffer_size Size of output buffer.
 * @return Number of characters written (excluding null terminator).
 */
uint32_t vc_alert_format_text(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size);

/**
 * @brief Format alert as JSON string.
 *
 * @param alert Alert to format.
 * @param buffer Output buffer.
 * @param buffer_size Size of output buffer.
 * @return Number of characters written (excluding null terminator).
 */
uint32_t vc_alert_format_json(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* VITALCORE_ALERT_H */

// Added a check to prevent null pointer dereferences in vc_generate_alert
vc_severity_t vc_generate_alert(const vc_vitals_t *vitals,
                                const vc_vitals_history_t *history,
                                const vc_alert_config_t *config,
                                vc_alert_t *alert)
{
    if (vitals == NULL || config == NULL || alert == NULL)
    {
        // Handle error, e.g., return a default severity or log an error
        return VC_SEVERITY_INFO;
    }

    // Rest of the function remains the same
    // ...
}

// Improved error handling in vc_alert_format_text and vc_alert_format_json
uint32_t vc_alert_format_text(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size)
{
    if (alert == NULL || buffer == NULL)
    {
        // Handle error, e.g., return 0 or log an error
        return 0;
    }

    // Rest of the function remains the same
    // ...
}

uint32_t vc_alert_format_json(const vc_alert_t *alert,
                              char *buffer,
                              uint32_t buffer_size)
{
    if (alert == NULL || buffer == NULL)
    {
        // Handle error, e.g., return 0 or log an error
        return 0;
    }

    // Rest of the function remains the same
    // ...
}