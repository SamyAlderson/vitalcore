#ifndef VITALCORE_VITALS_H
#define VITALCORE_VITALS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

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
    vc_vitals_t readings[1];  /**< Array of historical readings */
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
vc_vitals_history_t *vc_vitals_history_create(uint32_t capacity, uint32_t window_minutes) {
    size_t total_size = sizeof(vc_vitals_history_t) + (capacity - 1) * sizeof(vc_vitals_t);
    vc_vitals_history_t *history = malloc(total_size);
    if (history == NULL) {
        return NULL;
    }
    if (total_size > UINT32_MAX) {
        free(history);
        return NULL;
    }
    history->count = 0;
    history->capacity = capacity;
    history->window_minutes = window_minutes;
    return history;
}

/**
 * @brief Free a vitals history buffer.
 *
 * @param history Pointer to history to free.
 */
void vc_vitals_history_destroy(vc_vitals_history_t *history) {
    if (history != NULL) {
        free(history);
    }
}

/**
 * @brief Add a reading to the history buffer.
 *
 * @param history Pointer to history.
 * @param vitals Vital signs reading to add.
 * @return 0 on success, -1 on failure.
 */
int vc_vitals_history_add_reading(vc_vitals_history_t *history, vc_vitals_t *vitals) {
    if (history == NULL || vitals == NULL) {
        return -1;
    }
    if (history->count >= history->capacity) {
        return -1;
    }
    history->readings[history->count] = *vitals;
    history->count++;
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif