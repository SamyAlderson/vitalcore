/**
 * @file vitals.c
 * @brief Vital signs data management implementation.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "vitalcore/vitals.h"

#define NaN __builtin_nanf("")

vc_vitals_t vc_vitals_default(void) {
    vc_vitals_t v;
    memset(&v, 0, sizeof(v));
    v.heart_rate = 0.0f;
    v.spo2 = 0.0f;
    v.systolic = 0.0f;
    v.diastolic = 0.0f;
    v.temperature = 0.0f;
    v.respiratory_rate = 0.0f;
    v.mean_arterial = 0.0f;
    v.timestamp = 0;
    v.age = VC_AGE_ADULT;
    v.gender = VC_GENDER_UNKNOWN;
    return v;
}

bool vc_vitals_is_present(float value) {
    return value > 0.0f && !isnan(value) && !isinf(value);
}

float vc_calculate_map(float systolic, float diastolic) {
    if (systolic <= 0.0f || diastolic <= 0.0f) {
        return 0.0f;
    }
    return diastolic + (systolic - diastolic) / 3.0f;
}

vc_vitals_history_t *vc_vitals_history_create(uint32_t capacity, uint32_t window_minutes) {
    if (capacity == 0) return NULL;

    vc_vitals_history_t *h = (vc_vitals_history_t *)calloc(1, sizeof(vc_vitals_history_t));
    if (!h) return NULL;

    h->readings = (vc_vitals_t *)calloc(capacity, sizeof(vc_vitals_t));
    if (!h->readings) {
        free(h);
        return NULL;
    }

    h->capacity = capacity;
    h->count = 0;
    h->window_minutes = window_minutes;
    return h;
}

void vc_vitals_history_destroy(vc_vitals_history_t *history) {
    if (!history) return;
    free(history->readings);
    free(history);
}

bool vc_vitals_history_add(vc_vitals_history_t *history, const vc_vitals_t *vitals) {
    if (!history || !vitals) return false;

    if (history->count < history->capacity) {
        history->readings[history->count] = *vitals;
        if (history->readings[history->count].timestamp == 0) {
            history->readings[history->count].timestamp = (int64_t)time(NULL);
        }
        history->count++;
    } else {
        /* Shift left (ring buffer style) */
        memmove(&history->readings[0], &history->readings[1],
                (history->capacity - 1) * sizeof(vc_vitals_t));
        history->readings[history->capacity - 1] = *vitals;
        if (history->readings[history->capacity - 1].timestamp == 0) {
            history->readings[history->capacity - 1].timestamp = (int64_t)time(NULL);
        }
    }
    return true;
}

const vc_vitals_t *vc_vitals_history_latest(const vc_vitals_history_t *history) {
    if (!history || history->count == 0) return NULL;
    return &history->readings[history->count - 1];
}
