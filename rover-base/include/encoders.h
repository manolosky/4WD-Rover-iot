#pragma once
#include <Arduino.h>
#include "config.h"

// ============================================================
// encoders.h — 4 encoder inputs (channel A) via interrupts
// RPM and distance per wheel. Direction comes from Motors.
// ============================================================

namespace Encoders {

inline volatile uint32_t pulses[4] = {0, 0, 0, 0};
inline uint32_t lastPulses[4] = {0, 0, 0, 0};
inline float rpm[4] = {0, 0, 0, 0};
inline float totalDistanceMM = 0;   // average of the 4 wheels
inline uint32_t lastCalcMs = 0;

void IRAM_ATTR isrM1() { pulses[0]++; }
void IRAM_ATTR isrM2() { pulses[1]++; }
void IRAM_ATTR isrM3() { pulses[2]++; }
void IRAM_ATTR isrM4() { pulses[3]++; }

inline void begin() {
  pinMode(PIN_ENC_M1, INPUT_PULLUP);
  pinMode(PIN_ENC_M2, INPUT_PULLUP);
  pinMode(PIN_ENC_M3, INPUT);   // 36 and 39 have no internal pull-up:
  pinMode(PIN_ENC_M4, INPUT);   // the Hall encoder already outputs a clean signal
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M1), isrM1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M2), isrM2, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M3), isrM3, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_M4), isrM4, RISING);
  lastCalcMs = millis();
}

// Call every ENCODER_CALC_MS from the control task
inline void update() {
  uint32_t now = millis();
  uint32_t dt = now - lastCalcMs;
  if (dt < ENCODER_CALC_MS) return;
  lastCalcMs = now;

  float sumDeltaMM = 0;
  for (int i = 0; i < 4; i++) {
    uint32_t p = pulses[i];               // atomic copy (32-bit OK on ESP32)
    uint32_t delta = p - lastPulses[i];
    lastPulses[i] = p;
    // RPM = (pulses / PPR) / (dt in minutes)
    rpm[i] = (delta / ENC_PULSES_PER_REV) * (60000.0f / dt);
    sumDeltaMM += delta * MM_PER_PULSE;
  }
  totalDistanceMM += sumDeltaMM / 4.0f;
}

// Detect a stalled wheel: movement commanded but no pulses arriving
inline bool wheelStalled(uint8_t idx, bool commanded) {
  return commanded && rpm[idx] < 2.0f;
}

inline float avgRPM() { return (rpm[0] + rpm[1] + rpm[2] + rpm[3]) / 4.0f; }
inline float speedMS() { return avgRPM() * WHEEL_CIRCUM_MM / 60000.0f; }

} // namespace Encoders
