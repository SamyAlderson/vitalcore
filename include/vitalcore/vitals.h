#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    vc_vitals_history_t *history = malloc(sizeof(vc_vitals_history_t) + (capacity - 1) * sizeof(vc_vitals_t));
    if (history == NULL) {
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
        free(history->readings);
        free(history);
    }
}

/**
 * @brief Add a reading to the history buffer.
 *
 * @param history Pointer to history.
 * @param vitals Vital signs reading to add.
 * @return true on success, false if buffer is full.
 */
bool vc_vitals_history_add(vc_vitals_history_t *history, const vc_vitals_t *vitals) {
    if (history == NULL || vitals == NULL) {
        return false;
    }
    if (history->count >= history->capacity) {
        return false;
    }
    if (history->count >= (sizeof(history->readings) / sizeof(history->readings[0]))) {
        // buffer overflow
        return false;
    }
    memcpy(&history->readings[history->count], vitals, sizeof(vc_vitals_t));
    history->count++;
    return true;
}

/**
 * @brief Get the latest reading from history.
 *
 * @param history Pointer to history.
 * @return Pointer to latest reading, or NULL if empty.
 */
const vc_vitals_t *vc_vitals_history_latest(const vc_vitals_history_t *history) {
    if (history == NULL || history->count == 0) {
        return NULL;
    }
    if (history->count > 0) {
        return &history->readings[history->count - 1];
    }
    return NULL; // added this to handle count == 0
}

/**
 * @brief Get the oldest reading from history.
 *
 * @param history Pointer to history.
 * @return Pointer to oldest reading, or NULL if empty.
 */
const vc_vitals_t *vc_vitals_history_oldest(const vc_vitals_history_t *history) {
    if (history == NULL || history->count == 0) {
        return NULL;
    }
    return &history->readings[0];
}

#ifdef __cplusplus
}
#endif