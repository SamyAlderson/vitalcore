#ifndef VITALCORE_ANOMALY_H
#define VITALCORE_ANOMALY_H

#include "vitals.h"
#include "thresholds.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file anomaly.h
 * @brief Anomaly detection for vital signs.
 *
 * Detects clinical anomalies in vital signs data using
 * evidence-based medical rules and pattern recognition.
 */

/**
 * @brief Type of anomaly detected.
 */
typedef enum {
    VC_ANOMALY_NONE = 0,

    /* Cardiac */
    VC_ANOMALY_BRADYCARDIA,         /**< Heart rate too low */
    VC_ANOMALY_TACHYCARDIA,         /**< Heart rate too high */

    /* Respiratory */
    VC_ANOMALY_HYPOXIA,             /**< SpO2 too low */
    VC_ANOMALY_BRADYPNEA,           /**< Respiratory rate too low */
    VC_ANOMALY_TACHYPNEA,           /**< Respiratory rate too high */

    /* Blood pressure */
    VC_ANOMALY_HYPOTENSION,         /**< Blood pressure too low */
    VC_ANOMALY_HYPERTENSION,        /**< Blood pressure too high */
    VC_ANOMALY_PULSE_PRESSURE_LOW,  /**< Narrow pulse pressure (shock indicator) */

    /* Temperature */
    VC_ANOMALY_FEVER,               /**< Body temperature too high */
    VC_ANOMALY_HYPOTHERMIA,         /**< Body temperature too low */

    /* Combined patterns */
    VC_ANOMALY_SEPSIS_SCREEN,       /**< qSOFA-like sepsis screening */
    VC_ANOMALY_SHOCK_INDEX_HIGH,    /**< Shock index > 1.0 (HR/SBP) */
    VC_ANOMALY_DESAT_TACHYPNEA,     /**< Combined low SpO2 + high RR */

    VC_ANOMALY_COUNT
} vc_anomaly_type_t;

/**
 * @brief Severity of detected anomaly.
 */
typedef enum {
    VC_SEVERITY_INFO = 0,       /**< Informational, within normal */
    VC_SEVERITY_WARNING,        /**< Abnormal, monitor closely */
    VC_SEVERITY_CRITICAL,       /**< Critical, immediate attention */
    VC_SEVERITY_EMERGENCY       /**< Life-threatening, act now */
} vc_severity_t;

/**
 * @brief Detected anomaly with context.
 */
typedef struct {
    vc_anomaly_type_t type;     /**< Type of anomaly */
    vc_severity_t severity;     /**< Severity level */
    const char *name;           /**< Human-readable name */
    const char *description;    /**< Clinical description */
    float value;                /**< The value that triggered the anomaly */
    float threshold;            /**< The threshold that was exceeded */
} vc_anomaly_t;

/**
 * @brief Maximum number of anomalies per analysis.
 */
#define VC_MAX_ANOMALIES 16

/**
 * @brief Result of anomaly analysis.
 */
typedef struct {
    vc_anomaly_t anomalies[VC_MAX_ANOMALIES]; /**< Detected anomalies */
    uint32_t count;                            /**< Number of anomalies detected */
    bool has_emergency;                        /**< true if any EMERGENCY severity */
    bool has_critical;                         /**< true if any CRITICAL severity */
} vc_anomaly_result_t;

#ifdef __cplusplus
}
#endif

#endif  // VITALCORE_ANOMALY_H