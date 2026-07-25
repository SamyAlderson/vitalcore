/**
 * @file risk.c
 * @brief Risk scoring for patient deterioration.
 *
 * Implements MEWS (Modified Early Warning Score) and custom
 * risk scoring algorithms for early detection of patient deterioration.
 */

#include "vitals.h"
#include "anomaly.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Risk level classification.
 */
typedef enum {
    VC_RISK_LOW = 0,        /**< Low risk, routine monitoring */
    VC_RISK_MODERATE,       /**< Moderate risk, increased monitoring */
    VC_RISK_HIGH,           /**< High risk, urgent assessment needed */
    VC_RISK_CRITICAL        /**< Critical risk, immediate intervention */
} vc_risk_level_t;

/**
 * @brief MEWS (Modified Early Warning Score) component scores.
 */
typedef struct {
    int heart_rate_score;       /**< -2 to +3 */
    int systolic_score;         /**< -2 to +3 */
    int respiratory_rate_score; /**< -2 to +3 */
    int temperature_score;      /**< -2 to +3 */
    int consciousness_score;    /**< 0 to 3 (AVPU, default 0) */
    int total;                  /**< Total MEWS score (0-14) */
    vc_risk_level_t risk_level; /**< Classified risk level */
} vc_mews_score_t;

/**
 * @brief Calculate MEWS score from vital signs.
 *
 * @param vitals Vital signs to score.
 * @return MEWS score with risk classification.
 */
vc_mews_score_t vc_calculate_mews(vc_vitals_t vitals);

/**
 * @brief Custom composite risk score (0-100).
 *
 * Combines anomaly severity, vital sign deviations, and
 * trend analysis into a single risk percentage.
 *
 * @param vitals Current vital signs.
 * @param history Optional history for trend analysis.
 * @param anomalies Optional anomaly result (can be NULL, will be computed).
 * @return Risk score from 0 (no risk) to 100 (critical).
 */
float vc_calculate_risk_score(vc_vitals_t vitals,
                              vc_vitals_history_t history,
                              vc_anomaly_result_t anomalies);

/**
 * @brief Get human-readable string for risk level.
 *
 * @param level Risk level.
 * @return String name of the risk level.
 */
char *vc_risk_level_name(vc_risk_level_t level);

/**
 * @brief Get recommended action for a risk level.
 *
 * @param level Risk level.
 * @return String describing recommended clinical action.
 */
char *vc_risk_level_action(vc_risk_level_t level);

#ifdef __cplusplus
}
#endif