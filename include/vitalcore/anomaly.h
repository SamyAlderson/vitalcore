#include "vitals.h"
#include "thresholds.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @brief Analyze vital signs for anomalies.
 *
 * @param vitals Vital signs to analyze.
 * @param history Optional history for trend analysis (can be NULL).
 * @param result Output: anomaly analysis result.
 * @return Number of anomalies detected.
 */
uint32_t vc_analyze(const vc_vitals_t *vitals,
                    const vc_vitals_history_t *history,
                    vc_anomaly_result_t *result) {
    uint32_t anomalies_detected = 0;
    for (uint32_t i = 0; i < VC_MAX_ANOMALIES; i++) {
        result->anomalies[i].type = VC_ANOMALY_NONE;
        result->anomalies[i].severity = VC_SEVERITY_INFO;
        result->anomalies[i].name = "";
        result->anomalies[i].description = "";
        result->anomalies[i].value = 0.0;
        result->anomalies[i].threshold = 0.0;
    }
    result->count = 0;
    result->has_emergency = false;
    result->has_critical = false;

    // TODO: implement anomaly detection logic here
    // for now, just return 0 anomalies detected
    return 0;
}

/**
 * @brief Get human-readable string for anomaly type.
 *
 * @param type Anomaly type.
 * @return String name of the anomaly type.
 */
const char *vc_anomaly_type_name(vc_anomaly_type_t type) {
    switch (type) {
        case VC_ANOMALY_BRADYCARDIA:
            return "Bradycardia";
        case VC_ANOMALY_TACHYCARDIA:
            return "Tachycardia";
        // ... add more cases for other anomaly types
        default:
            return "Unknown";
    }
}

/**
 * @brief Get human-readable string for severity level.
 *
 * @param severity Severity level.
 * @return String name of the severity level.
 */
const char *vc_severity_name(vc_severity_t severity) {
    switch (severity) {
        case VC_SEVERITY_INFO:
            return "Informational";
        case VC_SEVERITY_WARNING:
            return "Warning";
        case VC_SEVERITY_CRITICAL:
            return "Critical";
        case VC_SEVERITY_EMERGENCY:
            return "Emergency";
        default:
            return "Unknown";
    }
}

#ifdef __cplusplus
}
#endif