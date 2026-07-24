/**
 * @file basic_monitor.c
 * @brief Example: Basic vital signs monitoring with VitalCore.
 */

#include <stdio.h>
#include <vitalcore/vitalcore.h>

static void print_alert(const vc_alert_t *alert) {
    char buffer[2048];
    vc_alert_format_text(alert, buffer, sizeof(buffer));
    printf("%s\n", buffer);
}

int main(void) {
    printf("VitalCore v%s — Basic Monitor Example\n", vc_version());
    printf("======================================\n\n");

    /* Create monitor */
    vc_monitor_t *monitor = vc_monitor_create();
    if (!monitor) {
        fprintf(stderr, "Failed to create monitor\n");
        return 1;
    }

    /* Scenario 1: Normal patient */
    printf("--- Scenario 1: Normal Patient ---\n");
    vc_vitals_t normal = vc_vitals_default();
    normal.heart_rate = 72.0f;
    normal.spo2 = 98.0f;
    normal.systolic = 120.0f;
    normal.diastolic = 80.0f;
    normal.temperature = 36.8f;
    normal.respiratory_rate = 16.0f;

    vc_alert_t alert;
    vc_severity_t sev = vc_monitor_analyze(monitor, &normal, &alert);
    printf("Severity: %s\n\n", vc_severity_name(sev));

    /* Scenario 2: Hypoxic patient */
    printf("--- Scenario 2: Hypoxic Patient ---\n");
    vc_vitals_t hypoxic = vc_vitals_default();
    hypoxic.heart_rate = 110.0f;
    hypoxic.spo2 = 85.0f;
    hypoxic.systolic = 95.0f;
    hypoxic.diastolic = 60.0f;
    hypoxic.temperature = 37.2f;
    hypoxic.respiratory_rate = 28.0f;

    sev = vc_monitor_analyze(monitor, &hypoxic, &alert);
    print_alert(&alert);

    /* Scenario 3: Sepsis screening */
    printf("--- Scenario 3: Possible Sepsis ---\n");
    vc_vitals_t sepsis = vc_vitals_default();
    sepsis.heart_rate = 125.0f;
    sepsis.spo2 = 91.0f;
    sepsis.systolic = 82.0f;
    sepsis.diastolic = 50.0f;
    sepsis.temperature = 39.8f;
    sepsis.respiratory_rate = 30.0f;

    sev = vc_monitor_analyze(monitor, &sepsis, &alert);
    print_alert(&alert);

    /* Scenario 4: Cardiac emergency */
    printf("--- Scenario 4: Cardiac Emergency ---\n");
    vc_vitals_t cardiac = vc_vitals_default();
    cardiac.heart_rate = 35.0f;
    cardiac.spo2 = 88.0f;
    cardiac.systolic = 70.0f;
    cardiac.diastolic = 45.0f;
    cardiac.temperature = 35.5f;
    cardiac.respiratory_rate = 8.0f;

    sev = vc_monitor_analyze(monitor, &cardiac, &alert);
    print_alert(&alert);

    /* JSON output */
    printf("--- JSON Output (Last Alert) ---\n");
    char json[2048];
    vc_alert_format_json(&alert, json, sizeof(json));
    printf("%s\n", json);

    /* Cleanup */
    vc_monitor_destroy(monitor);

    printf("\nDone. All scenarios processed.\n");
    return 0;
}
