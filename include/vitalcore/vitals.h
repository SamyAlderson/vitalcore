/**
 * @file vitals.h
 * @brief Vital signs data structures for VitalCore.
 *
 * Defines the core data types for representing patient vital signs.
 * All values use standard clinical units (bpm, %, mmHg, °C, breaths/min).
 */

#ifndef VITALCORE_VITALS_H
#define VITALCORE_VITALS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Age group classification for threshold selection.
 */
typedef enum {
    VC_AGE_ADULT,       /**< Adult (>= 18 years) */
    VC_AGE_CHILD,       /**< Child (1-17 years) */
    VC_AGE_INFANT       /**< Infant (< 1 year) */
} vc_age_group_t;

/**
 * @brief Gender classification.
 */
typedef enum {
    VC_GENDER_UNKNOWN,
    VC_GENDER_MALE,
    VC_GENDER_FEMALE
} vc_gender_t;

/**
 * @brief Single vital signs measurement.
 *
 * All fields are optional (use NaN or 0 to indicate missing).
 * At minimum, heart_rate should be provided for basic monitoring.
 */
typedef struct {
    float heart_rate;       /**< Heart rate in bpm (beats per minute) */
    float spo2;             /**< Blood oxygen saturation in % (0-100) */
    float systolic;         /**< Systolic blood pressure in mmHg */
    float diastolic;        /**< Diastolic blood pressure in mmHg */
    float temperature;      /**< Body temperature in °C */
    float respiratory_rate; /**< Respiratory rate in breaths per minute */
    float mean_arterial;    /**< Mean arterial pressure in mmHg (auto-calculated if 0) */
    int64_t timestamp;      /**< Unix timestamp in seconds (0 = now) */
    vc_age_group_t age;     /**< Age group for threshold selection */
    vc_gender_t gender;     /**< Gender */
} vc_vitals_t;

/**
 * @brief Vital signs trends over time (for pattern detection).
 */
typedef struct {
    vc_vitals_t *readings;  /**< Array of historical readings */
    uint32_t count;         /**< Number of readings */
    uint32_t capacity;      /**< Allocated capacity */
    uint32_t window_minutes;/**< Time window in minutes for trend analysis */
} vc_vitals_history_t;

/**
 * @brief Create a new vitals history buffer.
 *
 * @param capacity Maximum number of readings to store.
 * @param window_minutes Time window for trend analysis.
 * @return Pointer to allocated history, or NULL on failure.
 */
vc_vitals_history_t *vc_vitals_history_create(uint32_t capacity, uint32_t window_minutes);

/**
 * @brief Free a vitals history buffer.
 *
 * @param history Pointer to history to free.
 */
void vc_vitals_history_destroy(vc_vitals_history_t *history);

/**
 * @brief Add a reading to the history buffer.
 *
 * @param history Pointer to history.
 * @param vitals Vital signs reading to add.
 * @return true on success, false if buffer is full.
 */
bool vc_vitals_history_add(vc_vitals_history_t *history, const vc_vitals_t *vitals);

/**
 * @brief Get the latest reading from history.
 *
 * @param history Pointer to history.
 * @return Pointer to latest reading, or NULL if empty.
 */
const vc_vitals_t *vc_vitals_history_latest(const vc_vitals_history_t *history);

/**
 * @brief Calculate mean arterial pressure from systolic/diastolic.
 *
 * @param systolic Systolic pressure in mmHg.
 * @param diastolic Diastolic pressure in mmHg.
 * @return Mean arterial pressure in mmHg.
 */
float vc_calculate_map(float systolic, float diastolic);

/**
 * @brief Create a default vitals structure with zeroed values.
 *
 * @return Zeroed vitals structure.
 */
vc_vitals_t vc_vitals_default(void);

/**
 * @brief Check if a vital sign value is present (not NaN/zero).
 *
 * @param value The vital sign value.
 * @return true if the value is present and valid.
 */
bool vc_vitals_is_present(float value);

#ifdef __cplusplus
}
#endif

#endif /* VITALCORE_VITALS_H */
