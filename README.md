# VitalCore

**Real-time vital signs monitoring and clinical alerting in C.**

VitalCore is an embeddable, high-performance C library for monitoring patient vital signs, detecting clinical anomalies, and generating real-time alerts. Designed for resource-constrained environments where Python/ML frameworks are too heavy.

## Why VitalCore?

- **1 billion people** lack access to a physician within a 2-hour travel radius
- Existing tools require cloud, GPU, or Python — impossible on a $10 device in rural areas
- VitalCore runs on **anything with a C compiler**: Raspberry Pi, microcontrollers, old laptops, embedded systems
- **Zero dependencies** beyond libc. No cloud. No internet required.

## Features

- **Vital Signs Parsing** — Heart rate, SpO2, blood pressure, temperature, respiratory rate
- **Clinical Thresholds** — WHO and AHA evidence-based thresholds for adults and children
- **Anomaly Detection** — Real-time detection of: bradycardia, tachycardia, hypoxia, hypotension, hypertension, fever, hypothermia, abnormal respiratory rate
- **Risk Scoring** — MEWS (Modified Early Warning Score) and custom risk computation
- **Alert Engine** — Severity-graded alerts (INFO, WARNING, CRITICAL, EMERGENCY)
- **Portable API** — Clean C99 API, linkable as static or shared library
- **100% Test Coverage** — Comprehensive unit tests with CTest

## Quick Start

```c
#include <vitalcore/vitalcore.h>

int main(void) {
    vc_monitor_t *monitor = vc_monitor_create();
    
    vc_vitals_t vitals = {
        .heart_rate = 45,
        .spo2 = 88,
        .systolic = 85,
        .diastolic = 55,
        .temperature = 39.2,
        .respiratory_rate = 28
    };
    
    vc_alert_t alert;
    vc_monitor_analyze(monitor, &vitals, &alert);
    
    if (alert.severity >= VC_SEVERITY_CRITICAL) {
        printf("CRITICAL: %s\n", alert.message);
    }
    
    vc_monitor_destroy(monitor);
    return 0;
}
```

## Build

```bash
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

## Architecture

```
vitalcore/
├── include/vitalcore/
│   ├── vitalcore.h      # Main API
│   ├── vitals.h         # Vital signs data structures
│   ├── thresholds.h     # Clinical thresholds (WHO/AHA)
│   ├── anomaly.h        # Anomaly detection
│   ├── risk.h           # Risk scoring (MEWS)
│   └── alert.h          # Alert engine
├── src/
│   ├── vitals.c         # Vital signs management
│   ├── thresholds.c     # Threshold evaluation
│   ├── anomaly.c        # Anomaly detection algorithms
│   ├── risk.c           # Risk score computation
│   ├── alert.c          # Alert generation
│   └── vitalcore.c      # High-level API
├── tests/               # Unit tests (CTest)
├── examples/            # Usage examples
└── docs/                # Documentation
```

## Clinical References

- **WHO** — Emergency triage guidelines
- **AHA** — American Heart Association vital sign ranges
- **MEWS** — Modified Early Warning Score (Subbe et al., 2001)
- **qSOFA** — Quick Sequential Organ Failure Assessment

## License

MIT — See [LICENSE](LICENSE)

## Authors

- **SamyAlderson** — Core development
- **Edouard** — Co-author, code review, bug fixes
