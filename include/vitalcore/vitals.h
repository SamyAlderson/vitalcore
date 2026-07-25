#ifndef VITALCORE_VITALS_H
#define VITALCORE_VITALS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VC_AGE_ADULT,
    VC_AGE_CHILD,
    VC_AGE_INFANT
} vc_age_group_t;

typedef enum {
    VC_GENDER_UNKNOWN,
    VC_GENDER_MALE,
    VC_GENDER_FEMALE
} vc_gender_t;

typedef struct {
    float heart_rate;
    float spo2;
    float systolic;
    float diastolic;
    float temperature;
    float respiratory_rate;
    float mean_arterial;
    int64_t timestamp;
    vc_age_group_t age;
    vc_gender_t gender;
} vc_vitals_t;

typedef struct {
    vc_vitals_t readings[1];
    uint32_t count;
    uint32_t capacity;
    uint32_t window_minutes;
} vc_vitals_history_t;

vc_vitals_history_t *vc_vitals_history_create(uint32_t capacity, uint32_t window_minutes) {
    if (capacity == 0 || window_minutes == 0) {
        return NULL;
    }
    vc_vitals_history_t *history = malloc(sizeof(vc_vitals_history_t));
    if (history == NULL) {
        return NULL;
    }
    history->capacity = capacity;
    history->window_minutes = window_minutes;
    history->count = 0;
    return history;
}

void vc_vitals_history_destroy(vc_vitals_history_t *history) {
    if (history != NULL) {
        free(history);
    }
}

bool vc_vitals_history_add(vc_vitals_history_t *history, const vc_vitals_t *vitals) {
    if (history == NULL || vitals == NULL) {
        return false;
    }
    if (history->count >= history->capacity) {
        return false;
    }
    history->readings[history->count] = *vitals;
    history->readings[history->count].timestamp = time(NULL);
    history->count++;
    return true;
}

const vc_vitals_t *vc_vitals_history_latest(const vc_vitals_history_t *history) {
    if (history == NULL || history->count == 0) {
        return NULL;
    }
    return &history->readings[history->count - 1];
}

const vc_vitals_t *vc_vitals_history_oldest(const vc_vitals_history_t *history) {
    if (history == NULL || history->count == 0) {
        return NULL;
    }
    return &history->readings[0];
}

vc_vitals_t vc_vitals_default(void) {
    vc_vitals_t vitals;
    vitals.heart_rate = 0.0;
    vitals.spo2 = 0.0;
    vitals.systolic = 0.0;
    vitals.diastolic = 0.0;
    vitals.temperature = 0.0;
    vitals.respiratory_rate = 0.0;
    vitals.mean_arterial = 0.0;
    vitals.timestamp = 0;
    vitals.age = VC_AGE_INFANT;
    vitals.gender = VC_GENDER_UNKNOWN;
    return vitals;
}

bool vc_vitals_is_present(float value) {
    return value > 0.0;
}

float vc_calculate_map(float systolic, float diastolic) {
    if (systolic <= 0.0 || diastolic <= 0.0) {
        return 0.0;
    }
    return (systolic + diastolic) / 2.0;
}

#ifdef __cplusplus
}
#endif

#endif /* VITALCORE_VITALS_H */